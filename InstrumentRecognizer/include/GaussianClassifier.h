#pragma once

#include <Classifier.h>
#include <Discretizer.h>
#include <DiscretizerFactory.h>

class GaussianClassifier : public Classifier
{
public:

	GaussianClassifier();

	class Builder : public Classifier::Builder
	{
	public:
		Builder();

		virtual GaussianClassifier* build();
		virtual GaussianClassifier* build(const std::string&);
		virtual GaussianClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);

	private:
		std::shared_ptr<DiscretizerFactory> factory;
	};

	virtual void accept(ClassifierVisitor* visitor);

private:
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

	double probablilityFor(const ClassName&, const ObjectDescription&) const;

	void teach();
	
	std::vector<std::map<Discretized, std::map<ClassName, double> > > probabilities;
};