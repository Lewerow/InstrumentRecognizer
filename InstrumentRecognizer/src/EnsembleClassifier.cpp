#include <EnsembleClassifier.h>

#include "OwnNearestNeighborClassifier.h"
#include "ClassifierVisitor.h"
#include "ClassifierObservers.h"

#include <numeric>
#include <map>
#include <random>

class RandomDataResamplingClassifierObserver : public ClassifierObserver
{
public:
	RandomDataResamplingClassifierObserver(double part_to_leave = 1) : leave(part_to_leave)
	{}
	
	void notifyReceivedNewData(Classifier* c, ClassDescriptionBase& base)
	{
		ClassDescriptionBase b;
		std::size_t total = 0;
		for (auto c : base)
			total += c.second.size();

		std::size_t to_leave = total * leave;

		std::random_device dev;
		std::mt19937 generator(dev());
		std::uniform_int_distribution<std::size_t> dist(0, total - 1);
		for (std::size_t i = 0; i < to_leave; ++i)
		{
			std::size_t next = dist(generator);
			std::size_t max = 0;

			ClassDescriptionBase::iterator it = base.begin();
			while (next >= max + it->second.size())
			{
				max += it->second.size();
				++it;
			}

			b[it->first].push_back(it->second[next - max]);
		}

		base = b;
	}

	double leave;
};

BaggingEnsembleClassifier::BaggingEnsembleClassifier(BaggingEnsembleClassifier::Strategy str) : strategy(str)
{}

BaggingEnsembleClassifier::~BaggingEnsembleClassifier()
{}

void BaggingEnsembleClassifier::doRun()
{
	auto resampler = std::make_shared<RandomDataResamplingClassifierObserver>();
	for (std::size_t i = 0; i < strategy.k; ++i)
	{
		std::unique_ptr<Classifier> c(strategy.elementMaker->build());
		c->attachObserver(resampler);
		c->setInputData(descriptionBase);
		c->run();

		elements.push_back(std::move(c));
	}
}

void BaggingEnsembleClassifier::doStop()
{}

void BaggingEnsembleClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

BaggingEnsembleClassifier::Builder* BaggingEnsembleClassifier::doMakeBuilder()
{
	return new Builder(strategy);
}

ClassifierResults BaggingEnsembleClassifier::doCalculation(const ObjectDescription& desc_) const
{
	std::map<ClassName, std::size_t> occurrences;
	typedef std::pair<ClassName, std::size_t> Iter;

	for (auto& e : elements)
		++occurrences[e->classify(desc_)];

	return singleResultClass(std::max_element(occurrences.begin(), occurrences.end(), [](Iter i1, Iter i2){return i1.second > i2.second; })->first);
}

ClassName BaggingEnsembleClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

BaggingEnsembleClassifier::Builder::Builder(BaggingEnsembleClassifier::Strategy newStrategy) : strategy(newStrategy)
{}

BaggingEnsembleClassifier* BaggingEnsembleClassifier::Builder::build()
{
	return new BaggingEnsembleClassifier(strategy);
}

BaggingEnsembleClassifier* BaggingEnsembleClassifier::Builder::build(const std::string&)
{
	return new BaggingEnsembleClassifier(strategy);
}

BaggingEnsembleClassifier* BaggingEnsembleClassifier::Builder::build(const XMLNode&)
{
	return new BaggingEnsembleClassifier(strategy);
}

std::unique_ptr<XMLNode> BaggingEnsembleClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string BaggingEnsembleClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}

class WeightedResamplingClassifierObserver : public ClassifierObserver
{
public:
	WeightedResamplingClassifierObserver(const std::map<ClassName, std::vector<double> >& weights_, double part_to_leave = 1) : leave(part_to_leave), weights(weights_)
	{}

	void notifyReceivedNewData(Classifier* c, ClassDescriptionBase& base)
	{
		ClassDescriptionBase b;
		double total = 0;
		std::size_t count = 0;
		for (auto c : weights)
		{
			total += std::accumulate(c.second.begin(), c.second.end(), 0.0);
			count += c.second.size();
		}
	
		std::size_t to_leave = count * leave;

		std::random_device dev;
		std::mt19937 generator(dev());
		std::uniform_real_distribution<double> dist(0, total);
		for (std::size_t i = 0; i < to_leave; ++i)
		{
			double next = dist(generator);
			double max = 0;

			std::map<ClassName, std::vector<double> >::const_iterator w = weights.begin();
			ClassDescriptionBase::iterator it = base.begin();
			while (next >= max + std::accumulate(weights.at(it->first).begin(), weights.at(it->first).end(), 0.0))
			{
				max += std::accumulate(weights.at(it->first).begin(), weights.at(it->first).end(), 0.0);
				++it;
			}

			std::size_t j = 0;
			while (next >= max + weights.at(it->first)[j])
			{
				max += weights.at(it->first)[j];
				++j;
			}

			b[it->first].push_back(it->second[j]);
		}

		base = b;
	}

	double leave;
	const std::map<ClassName, std::vector<double> > weights;
};

double getError(Classifier& c, const ClassDescriptionBase& b, const std::map<ClassName, std::vector<double> >& weights)
{
	double error = 0;
	for (auto& cls : b)
	{
		for (std::size_t i = 0; i < cls.second.size(); ++i)
		{
			if (c.classify(cls.second[i]) != cls.first)
				error += weights.at(cls.first)[i];
		}
	}

	return error;
}

void adaptWeights(Classifier& c, const ClassDescriptionBase& b, std::map<ClassName, std::vector<double> >& weights, double beta)
{
	double sum = 0;
	for (auto& cls : b)
	{
		for (std::size_t i = 0; i < cls.second.size(); ++i)
		{
			if (c.classify(cls.second[i]) != cls.first)
				weights[cls.first][i] /= std::sqrt(beta);
			else
				weights[cls.first][i] *= std::sqrt(beta);
			
			sum += weights[cls.first][i];
		}
	}

	for (auto& cls : b)
	{
		for (std::size_t i = 0; i < cls.second.size(); ++i)
			weights[cls.first][i] /= sum;
	}
}

BoostingEnsembleClassifier::BoostingEnsembleClassifier(BoostingEnsembleClassifier::Strategy str) : strategy(str)
{}

BoostingEnsembleClassifier::~BoostingEnsembleClassifier()
{}

void BoostingEnsembleClassifier::doRun()
{
	auto resampler = std::make_shared<WeightedResamplingClassifierObserver>(dataWeights);
	for (std::size_t i = 0; i < strategy.k; ++i)
	{
		std::unique_ptr<Classifier> c(strategy.elementMaker->build());
		c->attachObserver(resampler);
		c->setInputData(descriptionBase);
		c->run();

		double error = getError(*c, descriptionBase, dataWeights);
		double beta = error / (1 - error);
		if (error > 0.5)
		{
			double elems = 0;
			for (auto& c : descriptionBase)
				elems += c.second.size();

			for (auto& c : descriptionBase)
			for (auto x : c.second)
				dataWeights[c.first].push_back(1 / elems);
		}
		else
			adaptWeights(*c, descriptionBase, dataWeights, beta);

		elements.push_back(std::make_pair(std::move(c), std::log(1 / beta) / 2));

		if (error < 0.0000000001)
			break;
	}
}

void BoostingEnsembleClassifier::doStop()
{}

void BoostingEnsembleClassifier::doSetInputData(const ClassDescriptionBase& b)
{
	double elems = 0;
	for (auto& c : b)
		elems += c.second.size();

	for(auto& c: b)
		for (auto x : c.second)
			dataWeights[c.first].push_back(1 / elems);
}

void BoostingEnsembleClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

BoostingEnsembleClassifier::Builder* BoostingEnsembleClassifier::doMakeBuilder()
{
	return new Builder(strategy);
}

ClassifierResults BoostingEnsembleClassifier::doCalculation(const ObjectDescription& desc_) const
{
	std::map<ClassName, double> occurrences;
	typedef std::pair<ClassName, double> Iter;

	for (auto& e : elements)
		occurrences[e.first->classify(desc_)] += e.second;

	return singleResultClass(std::max_element(occurrences.begin(), occurrences.end(), [](Iter i1, Iter i2){return i1.second > i2.second; })->first);
}

ClassName BoostingEnsembleClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

BoostingEnsembleClassifier::Builder::Builder(BoostingEnsembleClassifier::Strategy newStrategy) : strategy(newStrategy)
{}

BoostingEnsembleClassifier* BoostingEnsembleClassifier::Builder::build()
{
	return new BoostingEnsembleClassifier(strategy);
}

BoostingEnsembleClassifier* BoostingEnsembleClassifier::Builder::build(const std::string&)
{
	return new BoostingEnsembleClassifier(strategy);
}

BoostingEnsembleClassifier* BoostingEnsembleClassifier::Builder::build(const XMLNode&)
{
	return new BoostingEnsembleClassifier(strategy);
}

std::unique_ptr<XMLNode> BoostingEnsembleClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string BoostingEnsembleClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}