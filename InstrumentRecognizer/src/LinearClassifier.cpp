#ifdef BUILD_WITH_SHARK
#include "LinearClassifier.h"
#include "ClassifierObservers.h"
#include "ClassifierVisitor.h"

LinearClassifier::LinearClassifier()
{}

LinearClassifier::~LinearClassifier()
{}

void LinearClassifier::doRun()
{
	shark::LDA lda;
	lda.train(classifier, sharkInput);
	stop();
}

void LinearClassifier::doStop()
{}

void LinearClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

LinearClassifier::Builder* LinearClassifier::doMakeBuilder()
{
	return new Builder;
}

void LinearClassifier::doSetInputData(const ClassDescriptionBase& base)
{
	setSharkInput(base);
}

ClassifierResults LinearClassifier::doCalculation(const ObjectDescription& desc) const
{
	return toResults(classifier(ValueConverter::convert<double>(desc)));
}

ClassName LinearClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");

	return res.top().first;
}

LinearClassifier* LinearClassifier::Builder::build()
{
	return new LinearClassifier;
}

LinearClassifier* LinearClassifier::Builder::build(const std::string&)
{
	return new LinearClassifier;
}

LinearClassifier* LinearClassifier::Builder::build(const XMLNode&)
{
	return new LinearClassifier;
}

std::unique_ptr<XMLNode> LinearClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string LinearClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}
#endif