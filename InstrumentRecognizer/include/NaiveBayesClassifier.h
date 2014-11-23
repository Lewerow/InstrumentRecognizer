#ifndef NaiveBayes_CLASSIFIER_H_fmeiowfm348csacsahnytjuykl9090ik7jy4t43t43yhrthfgfgbbvbvbufgui43gj54gt
#define NaiveBayes_CLASSIFIER_H_fmeiowfm348csacsahnytjuykl9090ik7jy4t43t43yhrthfgfgbbvbvbufgui43gj54gt

#include <Classifier.h>
#include <Discretizer.h>
#include <DiscretizerFactory.h>

class NaiveBayesClassifier : public Classifier
{
public:

	NaiveBayesClassifier(std::shared_ptr<DiscretizerFactory> disc);

	class Builder : public Classifier::Builder
	{
	public:
		Builder(DiscretizerFactory::Creator discretizer, std::size_t defaultClassCount);

		virtual NaiveBayesClassifier* build();
		virtual NaiveBayesClassifier* build(const std::string&);
		virtual NaiveBayesClassifier* build(const XMLNode&);
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

	double probablilityFor(const ClassName&, const DiscretizedObjectDescription&) const;

	void teach();
	void discretizeAll();

	std::shared_ptr<DiscretizerFactory> discretizerFactory;

	DiscretizedObjectDescription discretizeSingleObject(const ObjectDescription&) const;

	DiscretizedClassDescriptionBase discretizedBase;
	std::vector<std::unique_ptr<Discretizer> > discretizers;

	std::vector<std::map<Discretized, std::map<ClassName, double> > > probabilities;
	double minProbability;
};

#endif