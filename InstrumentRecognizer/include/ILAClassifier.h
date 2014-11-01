#ifndef ILA_CLASSIFIER_H_fmeiowfm3489gjtrgbfkdfmdsklmewof34iufgui43gj54gt
#define ILA_CLASSIFIER_H_fmeiowfm3489gjtrgbfkdfmdsklmewof34iufgui43gj54gt

#include <Classifier.h>
#include <Discretizer.h>
#include <DiscretizerFactory.h>

#include <ILARule.h>

class ILAClassifier : public Classifier
{
public:
	class Builder : public Classifier::Builder
	{
	public:
		virtual ILAClassifier* build();
		virtual ILAClassifier* build(const std::string&);
		virtual ILAClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);
	};

	virtual void accept(ClassifierVisitor* visitor);

private:
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

    void teach();
	void discretizeAll();
    DiscretizedObjectDescription discretizeSingleObject(const ObjectDescription&) const;

	DiscretizedClassDescriptionBase discretizedBase;
	std::vector<std::unique_ptr<Discretizer> > discretizers;
    std::vector<ILARule> rules;
};

#endif