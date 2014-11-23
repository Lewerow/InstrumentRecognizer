#include <GaussianClassifier.h>
#include <ClassifierVisitor.h>
#include <Discretizer.h>

#include <numeric>

GaussianClassifier::GaussianClassifier()
{}

GaussianClassifier::Builder::Builder()
{}

GaussianClassifier* GaussianClassifier::Builder::build()
{
	return new GaussianClassifier;
}

GaussianClassifier* GaussianClassifier::Builder::build(const std::string&)
{
	return new GaussianClassifier;
}

GaussianClassifier* GaussianClassifier::Builder::build(const XMLNode&)
{
	return new GaussianClassifier;
}

std::unique_ptr<XMLNode> GaussianClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	// not supported
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string GaussianClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	// not supported
	return std::string();
}

GaussianClassifier::Builder* GaussianClassifier::doMakeBuilder()
{
	// not supported
	return nullptr;
}

ClassName GaussianClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

double GaussianClassifier::probablilityFor(const ClassName& cls, const ObjectDescription& desc) const
{
	double prob = 1;
	for (std::size_t att = 0; att < desc.size(); ++att)
	{
//		prob *= value->second.at(cls);
	}

	return prob;
}
ClassifierResults GaussianClassifier::doCalculation(const ObjectDescription& desc) const
{
	ClassifierResults res;
	for (auto& cls : descriptionBase)
		res.push(SingleClassifierResult(cls.first, probablilityFor(cls.first, desc)));

	return res;
}

void GaussianClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

void GaussianClassifier::doRun()
{
	teach();
}

void GaussianClassifier::doStop()
{}

void GaussianClassifier::teach()
{
}