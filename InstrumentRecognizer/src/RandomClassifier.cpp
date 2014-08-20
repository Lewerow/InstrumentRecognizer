#include "RandomClassifier.h"
#include "RandomServer.h"
#include "ClassifierVisitor.h"

RandomClassifier::RandomClassifier(std::shared_ptr<RandomServer> server) : randomServer(server)
{}

RandomClassifier::~RandomClassifier(void)
{}


void RandomClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

void RandomClassifier::doSetInputData(const ClassDescriptionBase&)
{}

RandomClassifier::Builder* RandomClassifier::doMakeBuilder()
{
	return new Builder(randomServer);
}

ClassName RandomClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

ClassifierResults RandomClassifier::doCalculation(const ObjectDescription&) const
{
	unsigned int chosenElem = randomServer->generate_uint(descriptionBase.size() - 1);
	auto it = descriptionBase.begin();
	for (unsigned int i = 0; i < chosenElem; ++i)
		++it;

	ClassifierResults res;
	res.push(SingleClassifierResult(it->first, 1.0));
	return res;
}

void RandomClassifier::doRun()
{
	stop();
}

void RandomClassifier::doStop()
{}

RandomClassifier::Builder::Builder(std::shared_ptr<RandomServer> rs) : randomServer(rs)
{}

RandomClassifier* RandomClassifier::Builder::build()
{
	return new RandomClassifier(randomServer);
}

RandomClassifier* RandomClassifier::Builder::build(const std::string&)
{
	return new RandomClassifier(randomServer);
}

RandomClassifier* RandomClassifier::Builder::build(const XMLNode&)
{
	return new RandomClassifier(randomServer);
}
std::unique_ptr<XMLNode> RandomClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	return nullptr;
}
std::string RandomClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	return std::string();
}