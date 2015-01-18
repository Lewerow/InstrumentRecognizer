#pragma once

#include "Classifier.h"
#include <map>

class BaggingEnsembleClassifier : public Classifier
{
public:
	struct Strategy
	{
		Strategy(std::size_t k_, std::shared_ptr<Classifier::Builder> b) : k(k_), elementMaker(b)
		{}

		std::size_t k;
		std::shared_ptr<Classifier::Builder> elementMaker;
	};

	class Builder : public Classifier::Builder
	{
	public:

		Builder(Strategy strategy);

		virtual BaggingEnsembleClassifier* build();
		virtual BaggingEnsembleClassifier* build(const std::string&);
		virtual BaggingEnsembleClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);

	private:
		Strategy strategy;
	};

	BaggingEnsembleClassifier(Strategy s);

	virtual ~BaggingEnsembleClassifier();

	virtual void accept(ClassifierVisitor* visitor);

private:
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

	Strategy strategy;
	std::vector<std::unique_ptr<Classifier> > elements;
};


class BoostingEnsembleClassifier : public Classifier
{
public:
	struct Strategy
	{
		Strategy(std::size_t k_, std::shared_ptr<Classifier::Builder> b) : k(k_), elementMaker(b)
		{}

		std::size_t k;
		std::shared_ptr<Classifier::Builder> elementMaker;
	};

	class Builder : public Classifier::Builder
	{
	public:

		Builder(Strategy strategy);

		virtual BoostingEnsembleClassifier* build();
		virtual BoostingEnsembleClassifier* build(const std::string&);
		virtual BoostingEnsembleClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);

	private:
		Strategy strategy;
	};

	BoostingEnsembleClassifier(Strategy s);

	virtual ~BoostingEnsembleClassifier();

	virtual void accept(ClassifierVisitor* visitor);

private:
	virtual void doSetInputData(const ClassDescriptionBase&);
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

	Strategy strategy;
	std::vector<std::pair<std::unique_ptr<Classifier>, double> > elements;
	std::map<ClassName, std::vector<double> > dataWeights;
};