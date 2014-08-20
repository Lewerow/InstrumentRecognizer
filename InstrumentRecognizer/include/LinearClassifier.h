#pragma once

#include "Classifier.h"
#include "SharkAdaptor.h"

#include <shark/Algorithms/Trainers/LDA.h>
#include <type_traits>

class LinearClassifier : public Classifier, private SharkAdaptor<>
{
	typedef shark::LinearClassifier<> Implementation;
	static_assert(std::is_same<Implementation::InputType, SharkInputValue>::value, "SharkInputValue and LinearClassifier InputType must be the same");
	static_assert(std::is_same<Implementation::OutputType, SharkInputKey>::value, "SharkInputKey and LinearClassifier OutputType must be the same");
	static_assert(std::is_same<Implementation::OutputType, OutputConverterInput>::value, "SharkOutput must be the same as LinearClassifier OutputType");

public:
	class Builder : public Classifier::Builder
	{
	public:
		virtual LinearClassifier* build();
		virtual LinearClassifier* build(const std::string&);
		virtual LinearClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);
	};

	LinearClassifier();
	virtual ~LinearClassifier();

	virtual void accept(ClassifierVisitor* visitor);
	
protected:
	Implementation classifier;

private:
	virtual void doSetInputData(const ClassDescriptionBase&);
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

};
