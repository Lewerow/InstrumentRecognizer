#include "OwnNearestNeighborClassifier.h"
#include "ClassifierVisitor.h"
#include "ClassifierObservers.h"

#include <numeric>
#include <map>

class Normalizer
{
public:
	Normalizer(const ClassDescriptionBase& base)
	{
		std::vector<std::pair<double, double> > minmax(base.begin()->second.front().size());

		for (auto& cls : base)
		{
			for (auto& obj : cls.second)
			{
				for (std::size_t i = 0; i < obj.size(); ++i)
				{
					if (obj[i] > minmax[i].second)
						minmax[i].second = obj[i];
					else if (obj[i] < minmax[i].first)
						minmax[i].first = obj[i];
				}
			}
		}

		scaleFactors.resize(minmax.size());
		for (std::size_t i = 0; i < minmax.size(); ++i)
			scaleFactors[i] = std::make_pair(-minmax[i].first, minmax[i].second - minmax[i].first);
	}

	ObjectDescription normalize(const ObjectDescription& d)
	{
		ObjectDescription res(d.size());
		std::transform(d.begin(), d.end(), scaleFactors.begin(), res.begin(), [](Descriptor d, std::pair<double, double> scale){return scale.first + d / scale.second; });
		return res;
	}

	std::vector<std::pair<double, double> > scaleFactors;
};

OwnNearestNeighborClassifier::OwnNearestNeighborClassifier(OwnNearestNeighborClassifier::Strategy str) : strategy(str)
{}

OwnNearestNeighborClassifier::~OwnNearestNeighborClassifier()
{}

void OwnNearestNeighborClassifier::doRun()
{
	normalizer = std::make_unique<Normalizer>(descriptionBase);
	for (auto cls : descriptionBase)
	{
		for (auto obj : cls.second)
			dataPoints.push_back(std::make_pair(cls.first, normalizer->normalize(obj)));
	}
}

double euclidDistance(const ObjectDescription& o1, const ObjectDescription& o2)
{
	double dist = 0;
	for (std::size_t i = 0; i < o1.size(); ++i)
		dist += std::pow(std::abs(o1[i] - o2[i]), 2);

	return std::sqrt(dist);
}

double manhattanDistance(const ObjectDescription& o1, const ObjectDescription& o2)
{
	double dist = 0;
	for (std::size_t i = 0; i < o1.size(); ++i)
		dist += std::abs(o1[i] - o2[i]);

	return dist;
}

double minkowski3Distance(const ObjectDescription& o1, const ObjectDescription& o2)
{
	double dist = 0;
	for (std::size_t i = 0; i < o1.size(); ++i)
		dist += std::pow(std::abs(o1[i] - o2[i]), 3);

	return std::cbrt(dist);
}

ClassName OwnNearestNeighborClassifier::majorityVoting(std::vector<std::pair<ClassName, ObjectDescription> >& neigh, const ObjectDescription& in, OwnNearestNeighborClassifier::DistanceMetric)
{
	std::map<ClassName, std::size_t> counts;

	for (int i = 0; i < neigh.size(); ++i)
		counts[neigh[i].first]++;

	return std::max_element(counts.begin(), counts.end(), [](std::pair<ClassName, std::size_t> p1, std::pair<ClassName, std::size_t> p2){return p1.second < p2.second; })->first;
}

ClassName OwnNearestNeighborClassifier::weightedVoting(std::vector<std::pair<ClassName, ObjectDescription> >& neigh, const ObjectDescription& in, OwnNearestNeighborClassifier::DistanceMetric dist)
{
	std::map<ClassName, double> counts;

	double last_dist = dist(neigh.back().second, in);
	double normalization_factor = dist(neigh.front().second, in) - last_dist;
	counts[neigh[0].first] = 1;
	for (int i = 1; i < neigh.size(); ++i)
		counts[neigh[i].first] += ((dist(neigh[i].second, in) - last_dist) / normalization_factor);

	return std::max_element(counts.begin(), counts.end(), [](std::pair<ClassName, std::size_t> p1, std::pair<ClassName, std::size_t> p2){return p1.second < p2.second; })->first;
}

// DWKNN according to "A Novel Weighted Voting for K-Nearest Neighbor Rule", Journal of Computers, Vol 6., No. 5, May 2011, Jianping Gou
ClassName OwnNearestNeighborClassifier::dualWeightedVoting(std::vector<std::pair<ClassName, ObjectDescription> >& neigh, const ObjectDescription& in, OwnNearestNeighborClassifier::DistanceMetric dist)
{
	std::map<ClassName, double> counts;

	double last_dist = dist(neigh.back().second, in);
	double normalization_factor = dist(neigh.front().second, in) - last_dist;
	counts[neigh[0].first] = 1;
	for (int i = 1; i < neigh.size(); ++i)
		counts[neigh[i].first] += ((dist(neigh[i].second, in) - last_dist) / (normalization_factor * i));

	return std::max_element(counts.begin(), counts.end(), [](std::pair<ClassName, std::size_t> p1, std::pair<ClassName, std::size_t> p2){return p1.second < p2.second; })->first;
}

void OwnNearestNeighborClassifier::doStop()
{}

void OwnNearestNeighborClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

OwnNearestNeighborClassifier::Builder* OwnNearestNeighborClassifier::doMakeBuilder()
{
	return new Builder(strategy);
}

ClassifierResults OwnNearestNeighborClassifier::doCalculation(const ObjectDescription& desc_) const
{
	ObjectDescription desc = normalizer->normalize(desc_);
	std::vector<std::pair<ClassName, ObjectDescription> > neighbors(strategy.k);
	std::partial_sort_copy(dataPoints.begin(), dataPoints.end(), neighbors.begin(), neighbors.end(), [this, desc](const std::pair<ClassName, ObjectDescription> i1, const std::pair<ClassName, ObjectDescription> i2){
		return this->strategy.metric(desc, i1.second) < this->strategy.metric(desc, i2.second);
	});

	return singleResultClass(strategy.voting(neighbors, desc, strategy.metric));
}

ClassName OwnNearestNeighborClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

OwnNearestNeighborClassifier::Builder::Builder(OwnNearestNeighborClassifier::Strategy newStrategy) : strategy(newStrategy)
{}

OwnNearestNeighborClassifier* OwnNearestNeighborClassifier::Builder::build()
{
	return new OwnNearestNeighborClassifier(strategy);
}

OwnNearestNeighborClassifier* OwnNearestNeighborClassifier::Builder::build(const std::string&)
{
	return new OwnNearestNeighborClassifier(strategy);
}

OwnNearestNeighborClassifier* OwnNearestNeighborClassifier::Builder::build(const XMLNode&)
{
	return new OwnNearestNeighborClassifier(strategy);
}

std::unique_ptr<XMLNode> OwnNearestNeighborClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string OwnNearestNeighborClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}