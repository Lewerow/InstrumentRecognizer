#ifdef BUILD_WITH_SHARK
#pragma once

#include <shark/Algorithms/Trainers/LassoRegression.h>

#include "Classifier.h"
#include "SharkAdaptor.h"
#include "SharkConverters.h"

// Pretty much broken - doesn't work and I have currently no idea why.
// Moreover, I have no time to check it. 
class LASSOClassifier : public Classifier, private SharkAdaptor<Converters::UnsignedIntToSingleElementRealVector, Converters::SingleElementRealVectorToClassifierResults>
{
	typedef shark::LinearModel<> Implementation;
	static_assert(std::is_same<Implementation::InputType, SharkInputValue>::value, "SharkInputValue and LASSOClassifier InputType must be the same");
	static_assert(std::is_same<Implementation::OutputType, SharkInputKey>::value, "SharkInputKey and LASSOClassifier OutputType must be the same");
	static_assert(std::is_same<Implementation::OutputType, OutputConverterInput>::value, "SharkOutput must be the same as LASSOClassifier OutputType");

public:
	class Builder : public Classifier::Builder
	{
	public:
		virtual LASSOClassifier* build();
		virtual LASSOClassifier* build(const std::string&);
		virtual LASSOClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);
	};

	LASSOClassifier(double newLambda = 50000);
	void accept(ClassifierVisitor* visitor);

private:
	virtual void doSetInputData(const ClassDescriptionBase&);
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

	std::unique_ptr<Implementation> classifier;
	double lambda;
};
#endif