#include <memory>
#include "Classifier.h"

class RandomServer;

class RandomClassifier : public Classifier
{
public:

	class Builder : public Classifier::Builder
	{
	public:
		Builder(std::shared_ptr<RandomServer>);
		virtual ~Builder(){}

		virtual RandomClassifier* build();
		virtual RandomClassifier* build(const std::string&);
		virtual RandomClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);

	private:
		std::shared_ptr<RandomServer> randomServer;
	};

	RandomClassifier(std::shared_ptr<RandomServer>);
	virtual ~RandomClassifier(void);

	virtual void accept(ClassifierVisitor* visitor);
	
private:
	virtual void doSetInputData(const ClassDescriptionBase&);
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

	std::shared_ptr<RandomServer> randomServer;
}; 