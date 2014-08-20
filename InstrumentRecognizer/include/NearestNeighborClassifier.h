#pragma once

#include "Classifier.h"
#include "SharkAdaptor.h"

#include <shark/Models/NearestNeighborClassifier.h>
#include <type_traits>
#include <memory>

class NearestNeighborClassifier : public Classifier, private SharkAdaptor<>
{
public:
	typedef shark::NearestNeighborClassifier<shark::RealVector> Implementation;
	static_assert(std::is_same<Implementation::InputType, SharkInputValue>::value, "SharkInputValue and NearestNeighborClassifier InputType must be the same");
	static_assert(std::is_same<Implementation::OutputType, SharkInputKey>::value, "SharkInputKey and NearestNeighborClassifier OutputType must be the same");
	static_assert(std::is_same<Implementation::OutputType, OutputConverterInput>::value, "SharkOutput must be the same as NearestNeighborClassifier OutputType");

	class Strategy
	{
	public:
		typedef SharkInputType Input;
		typedef Implementation Model;
		typedef Model::NearestNeighbors Implementation;

		Strategy(unsigned int neighbors) : numberOfNeighbors(neighbors){}
		virtual ~Strategy(){}

		virtual void setInputData(const Input&) = 0;

		virtual Implementation* getAlgorithm() = 0;
		virtual Strategy* clone() = 0;

		unsigned int getNumberOfNeighbors()
			{return numberOfNeighbors;}

		Strategy(const Strategy&) = delete;
		Strategy(const Strategy&&) = delete;
		Strategy& operator=(const Strategy&) = delete;
		Strategy&& operator=(const Strategy&&) = delete;
	
	protected:
		unsigned int numberOfNeighbors;
	};

	class Builder : public Classifier::Builder
	{
	public:

		Builder(Strategy* strategy);

		virtual NearestNeighborClassifier* build();
		virtual NearestNeighborClassifier* build(const std::string&);
		virtual NearestNeighborClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);

	private:
		std::unique_ptr<Strategy> strategy;
	};

	NearestNeighborClassifier(Strategy* = nullptr);

	virtual ~NearestNeighborClassifier();

	virtual void accept(ClassifierVisitor* visitor);

	void setAlgorithm(Implementation::NearestNeighbors*);
	void setNeighborCount(unsigned int);



protected:
	std::unique_ptr<Implementation> classifier;
	std::unique_ptr<Strategy> strategy;

private:
	virtual void doSetInputData(const ClassDescriptionBase&);
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();
};