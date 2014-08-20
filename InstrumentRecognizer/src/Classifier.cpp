#include "Classifier.h"
#include "ClassifierObservers.h"
#include "ClassifierVisitor.h"

#include <algorithm>

Classifier::Classifier() : observers(new ClassifierObservers)
{
}

Classifier::~Classifier(void)
{
}	

void Classifier::run()
{
	if (observers.get() != nullptr)
		observers->notifyStartedTraining(this);
	
	doRun();
}
void Classifier::stop()
{
	doStop();
	
	if (observers.get() != nullptr)
		observers->notifyFinishedTraining(this);
}

void Classifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

ClassName Classifier::classify(const ObjectDescription& desc) const
{
	return classify(calculate(desc));
}

ClassifierResults Classifier::calculate(const ObjectDescription& desc) const
{
	return doCalculation(desc);
}

ClassName Classifier::classify(const ClassifierResults& res) const
{
	return doClassification(res);
}

std::unique_ptr<Classifier::Builder> Classifier::makeBuilder()
{
	return std::unique_ptr<Classifier::Builder>(doMakeBuilder());
}

void Classifier::setInputData(const ClassDescriptionBase& base)
{
	descriptionBase = base;
	if (observers.get())
		observers->notifyReceivedNewData(this, descriptionBase);
	
	doSetInputData(descriptionBase);
}

void Classifier::doSetInputData(const ClassDescriptionBase& base)
{}

void Classifier::attachObserver(std::shared_ptr<ClassifierObserver> o)
{
	observers->add(o);
}

void Classifier::detachObserver(std::shared_ptr<ClassifierObserver> o)
{
	observers->remove(o);
}