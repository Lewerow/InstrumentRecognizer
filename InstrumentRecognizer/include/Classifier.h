#pragma once

#include <string>
#include <memory>
#include <vector>

#include "ClassifierResults.h"
#include "common_defs.h"

class ClassifierObserver;
class ClassifierObservers;
class ClassifierVisitor;

class Classifier
{
public:
	class Builder
	{
	public:
		Builder(){}
		virtual ~Builder(){}

		virtual Classifier* build() = 0;
		virtual Classifier* build(const std::string&) = 0;
		virtual Classifier* build(const XMLNode&) = 0;
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>) = 0;
		virtual std::string dismantleToText(std::shared_ptr<Classifier>) = 0;
	};

	Classifier();
	virtual ~Classifier(void);

	virtual std::unique_ptr<Classifier::Builder> makeBuilder();
	
	void setInputData(const ClassDescriptionBase&);
	void run();
	void stop();

	ClassifierResults calculate(const ObjectDescription&) const;
	ClassName classify(const ObjectDescription& desc) const;
	ClassName classify(const ClassifierResults&) const;

	virtual void accept(ClassifierVisitor* visitor);

	void attachObserver(std::shared_ptr<ClassifierObserver>);
	void detachObserver(std::shared_ptr<ClassifierObserver>);

protected:
	
	ClassDescriptionBase descriptionBase;
	std::unique_ptr<ClassifierObservers> observers;

private:
	virtual void doSetInputData(const ClassDescriptionBase&);
	virtual Builder* doMakeBuilder() = 0;
	virtual ClassName doClassification(const ClassifierResults&) const = 0;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const = 0;
	virtual void doRun() = 0;
	virtual void doStop() = 0;
};


