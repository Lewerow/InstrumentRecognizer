#include "ClassifierVisitor.h"
#include "ClassifierObservers.h"

class GeneralDataClassifierObserver : public ClassifierObserver, public ClassifierVisitor
{
public:

	virtual void notifyStartedTraining(Classifier*);
	virtual void notifyFinishedTraining(Classifier*);

	virtual void visit(Classifier*);
	virtual void visit(ILAClassifier*);
	virtual void visit(LinearClassifier*);
	virtual void visit(RandomClassifier*);
	virtual void visit(NearestNeighborClassifier*);
	virtual void visit(LASSOClassifier*);

	std::string title;
	std::string header;
	std::string footer;
	std::chrono::time_point<std::chrono::system_clock> date;
};
