#ifdef BUILD_WITH_SHARK
#include "LASSOClassifier.h"
#include "ClassifierVisitor.h"

LASSOClassifier::LASSOClassifier(double newLambda) : lambda(newLambda)
{}

void LASSOClassifier::doSetInputData(const ClassDescriptionBase& base)
{
	setSharkInput(base);
	classifier.reset(new Implementation(base.size(), 1));
}

LASSOClassifier::Builder* LASSOClassifier::doMakeBuilder()
{
	return new Builder;
}

ClassName LASSOClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

ClassifierResults LASSOClassifier::doCalculation(const ObjectDescription& desc) const
{
	IR_ASSERT(classifier.get() != nullptr, "Must have a classifier to classify!");
	
	// this part is required, because converter doesn't know anything about the code
	// I'm starting to think this might have been not so good idea after all
	OutputConverterInput out = (*classifier)(ValueConverter::convert<double>(desc));
	
	// I have totally no idea why it is heare, but somehow it works better with it
	out[0] = -out[0];
	if (out[0] > code.size())
		out[0] = code.size() - 1;
	else if (out[0] < 0)
		out[0] = 0;
		
	return toResults(out);
}

void LASSOClassifier::doRun()
{
	IR_ASSERT(classifier.get() != nullptr, "Must have a classifier to classify!");
	shark::LassoRegression<> trainer(100, 0.001);

	trainer.train(*classifier, sharkInput);
	stop();
}

void LASSOClassifier::doStop()
{
}

void LASSOClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

LASSOClassifier* LASSOClassifier::Builder::build()
{
	return new LASSOClassifier;
}

LASSOClassifier* LASSOClassifier::Builder::build(const std::string&)
{
	return new LASSOClassifier;
}

LASSOClassifier* LASSOClassifier::Builder::build(const XMLNode&s)
{
	return new LASSOClassifier;
}

std::unique_ptr<XMLNode> LASSOClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return std::unique_ptr<XMLNode>(nullptr);
}
std::string LASSOClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}
#endif