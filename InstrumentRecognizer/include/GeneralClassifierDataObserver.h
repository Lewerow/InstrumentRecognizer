#include "ClassifierVisitor.h"
#include "ClassifierObservers.h"

class GeneralDataClassifierObserver : public ClassifierObserver, public ClassifierVisitor
{
public:

	virtual void notifyStartedTraining(Classifier*);
	virtual void notifyFinishedTraining(Classifier*);

	virtual void visit(Classifier*);
	virtual void visit(ILAClassifier*);
	virtual void visit(NaiveBayesClassifier*);
	virtual void visit(LinearClassifier*);
	virtual void visit(RandomClassifier*);
	virtual void visit(NearestNeighborClassifier*);
	virtual void visit(OwnNearestNeighborClassifier*);
	virtual void visit(LASSOClassifier*);
	virtual void visit(BaggingEnsembleClassifier*);
	virtual void visit(BoostingEnsembleClassifier*);
	
	std::string title;
	std::string header;
	std::string footer;
	std::chrono::time_point<std::chrono::system_clock> date;
};
