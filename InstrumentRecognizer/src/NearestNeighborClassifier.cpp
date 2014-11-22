#ifdef BUILD_WITH_SHARK
#include "NearestNeighborClassifier.h"
#include "NearestNeighborsStrategies.h"
#include "ClassifierVisitor.h"
#include "ClassifierObservers.h"

NearestNeighborClassifier::NearestNeighborClassifier(NearestNeighborClassifier::Strategy* str) : strategy(str), classifier(nullptr)
{
	if (strategy.get() == nullptr)
		strategy.reset(new StandardTreeStrategy(3));	
}

NearestNeighborClassifier::~NearestNeighborClassifier()
{}

void NearestNeighborClassifier::doRun()
{
	IR_ASSERT(strategy.get() != nullptr, "Strategy is required before running NearestNeighborClassifier");

	classifier.reset(new Implementation(strategy->getAlgorithm(), strategy->getNumberOfNeighbors()));
	stop();
}

void NearestNeighborClassifier::doStop()
{}

void NearestNeighborClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

NearestNeighborClassifier::Builder* NearestNeighborClassifier::doMakeBuilder()
{
	IR_ASSERT(strategy.get() != nullptr, "Cannot create builder without strategy for NearestNeighborClassifier");
	return new Builder(strategy->clone());
}

void NearestNeighborClassifier::doSetInputData(const ClassDescriptionBase& base)
{
	setSharkInput(base);

	IR_ASSERT(strategy.get() != nullptr, "Strategy is required before setting input in NearestNeighborClassifier");
	strategy->setInputData(sharkInput);
}

ClassifierResults NearestNeighborClassifier::doCalculation(const ObjectDescription& desc) const
{
	return toResults((*classifier)(ValueConverter::convert<double>(desc)));
}

ClassName NearestNeighborClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

NearestNeighborClassifier::Builder::Builder(NearestNeighborClassifier::Strategy* newStrategy) : strategy(newStrategy)
{}

NearestNeighborClassifier* NearestNeighborClassifier::Builder::build()
{
	return new NearestNeighborClassifier(strategy->clone());
}

NearestNeighborClassifier* NearestNeighborClassifier::Builder::build(const std::string&)
{
	return new NearestNeighborClassifier(strategy->clone());
}

NearestNeighborClassifier* NearestNeighborClassifier::Builder::build(const XMLNode&)
{
	return new NearestNeighborClassifier(strategy->clone());
}

std::unique_ptr<XMLNode> NearestNeighborClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string NearestNeighborClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}
#endif