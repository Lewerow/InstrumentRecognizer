#include "ClassifierObservers.h"
#include "RandomServer.h"
#include "TimeServer.h"
#include "ReportBuilder.h"

#include <algorithm>
#include <cmath>
#include <boost/math/special_functions/round.hpp>

void ClassifierObservers::add(std::shared_ptr<ClassifierObserver> o)
{
	IR_ASSERT(std::find(begin(), end(), o) == end(), "Trying to attach the same observer more than once!");
	push_back(o);
}

void ClassifierObservers::remove(std::shared_ptr<ClassifierObserver> o)
{
	auto place = std::find(begin(), end(), o);
	IR_ASSERT(place != end(), "Trying to detach not attached observer!");

	erase(place);
}

void ClassifierObservers::notifyReceivedNewData(Classifier* c, ClassDescriptionBase& b)
{
	for(auto& i: *this)
		i->notifyReceivedNewData(c, b);
}
void ClassifierObservers::notifyStartedTraining(Classifier* c)
{
	for(auto& i: *this)
		i->notifyStartedTraining(c);
}
void ClassifierObservers::notifyFinishedTraining(Classifier* c)
{
	for(auto& i: *this)
		i->notifyFinishedTraining(c);
}
void ClassifierObservers::notifyIteration(Classifier* c, unsigned int i)
{
	for(auto& it: *this)
		it->notifyIteration(c, i);
}

DataCutterClassifierObserver::DataCutterClassifierObserver(const double cutDataBy, std::shared_ptr<RandomServer> rs) : 
	cuttingFraction(cutDataBy), randomServer(rs)
{}


DataCutterClassifierObserver::~DataCutterClassifierObserver(void)
{

}

void DataCutterClassifierObserver::notifyReceivedNewData(Classifier* c, ClassDescriptionBase& base)
{	
	std::unordered_map<ClassName, std::size_t> finalSizes = getFinalSizes(base);
	unsigned int total(0);

	for(auto& cl: finalSizes)
	{
		ClassDescription& currentlyModified = base.at(cl.first);
		std::random_shuffle(currentlyModified.begin(), currentlyModified.end(), *randomServer);
		currentlyModified.resize(cl.second);
		
		total += cl.second;

		if(reportBuilder)
			reportBuilder.get()->addTrainingObjectCount(c, cl.first, cl.second);
	}

	if(reportBuilder)
		reportBuilder.get()->addTotalTrainingObjectCount(c, total);
}

std::unordered_map<ClassName, std::size_t> DataCutterClassifierObserver::getFinalSizes(const ClassDescriptionBase& base)
{
	
	std::vector<std::size_t> baseSizes(base.size());
	std::vector<std::size_t> baseSizesModified(base.size());

	std::size_t totalSize(0);
	std::size_t totalSizeModified(0);

	const double leaveFraction = 1 - cuttingFraction;
	int i = 0;
	for(auto& cl: base)
	{
		baseSizes[i] = cl.second.size();
		baseSizesModified[i] = std::size_t(cl.second.size() * leaveFraction);
		totalSizeModified += baseSizesModified[i];
		totalSize += cl.second.size();
		i++;
	}
	adjustSizes(totalSize, totalSizeModified, baseSizes, baseSizesModified);

	std::unordered_map<ClassName, std::size_t> result;
	int j = 0;
	for(auto& cl: base)
		result.insert(std::make_pair(cl.first, baseSizesModified[j++]));
	
	return result;
}

void DataCutterClassifierObserver::adjustSizes(std::size_t& totalSize, std::size_t& totalSizeModified, std::vector<std::size_t>& baseSizes, std::vector<std::size_t>& baseSizesModified)
{		
	
	const double leaveFraction = 1 - cuttingFraction;
	while(totalSizeModified > std::size_t(boost::math::iround(totalSize * leaveFraction)))
	{
		// 1 object per class - nothing to cut
		if(totalSizeModified == baseSizesModified.size())
			break;

		std::size_t index;
		do
		{
			index = randomServer->generate_uint(baseSizesModified.size() - 1);
		} while(baseSizesModified[index] <= 1);

		baseSizesModified[index]--;
		totalSizeModified--;
	}

	while(totalSizeModified < std::size_t(boost::math::iround(totalSize * leaveFraction)))
	{
		std::size_t index;
		do
		{
			index = randomServer->generate_uint(baseSizesModified.size() - 1);
		} while(baseSizesModified[index] >= baseSizes[index]);

		baseSizesModified[index]++;
		totalSizeModified++;
	}
}

class BoundaryClassifierObserver::Impl
{
public:
	Impl(std::shared_ptr<TimeServer> ts) : timeServer(ts)
	{}

	boost::optional<std::chrono::system_clock::duration> maxDuration;
	boost::optional<unsigned int> maxIterations;

	void setStart(Classifier* c)
	{
		iterations.insert(std::make_pair(c, 0));
		starts.insert(std::make_pair(c, timeServer->getCurrentTime()));
	}

	std::chrono::system_clock::duration getTrainingDuration(Classifier* c)
	{
		return timeServer->getCurrentTime() - starts.at(c);
	}

	~Impl() {}

	std::unordered_map<Classifier*, std::chrono::time_point<std::chrono::system_clock> > starts;
	std::unordered_map<Classifier*, unsigned int> iterations;

private:
	std::shared_ptr<TimeServer> timeServer;
};

BoundaryClassifierObserver::BoundaryClassifierObserver(std::shared_ptr<TimeServer> timeServer) : pimpl(new Impl(timeServer))
{}

BoundaryClassifierObserver::~BoundaryClassifierObserver()
{}

void BoundaryClassifierObserver::setMaxDuration(std::chrono::system_clock::duration maxDur)
{
	pimpl->maxDuration = maxDur;
}

void BoundaryClassifierObserver::setMaxIterations(unsigned int maxIterations)
{
	pimpl->maxIterations = maxIterations;
}
	
void BoundaryClassifierObserver::notifyStartedTraining(Classifier* c)
{
	pimpl->setStart(c);
}

void BoundaryClassifierObserver::notifyFinishedTraining(Classifier* c)
{
	if(reportBuilder)
	{
		reportBuilder.get()->addNumberOfIterations(c, pimpl->iterations.at(c));
		reportBuilder.get()->addTrainingDuration(c, pimpl->getTrainingDuration(c));
	}

	pimpl->starts.erase(c);
	pimpl->iterations.erase(c);
}

void BoundaryClassifierObserver::notifyIteration(Classifier* c, unsigned int i)
{
	pimpl->iterations.at(c) = i;

	if(pimpl->maxIterations)
	{
		if(i > pimpl->maxIterations.get())
			c->stop();
	}

	if(pimpl->maxDuration)
	{
		if(pimpl->getTrainingDuration(c) > pimpl->maxDuration)
			c->stop();
	}
}
