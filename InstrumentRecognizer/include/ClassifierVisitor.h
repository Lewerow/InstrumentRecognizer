class Classifier;
class LinearClassifier;
class RandomClassifier;
class NearestNeighborClassifier;
class OwnNearestNeighborClassifier;
class LASSOClassifier;
class ILAClassifier;
class NaiveBayesClassifier;
class BaggingEnsembleClassifier;
class BoostingEnsembleClassifier;

class ClassifierVisitor
{
public:
	virtual void visit(Classifier*) = 0;
	virtual void visit(ILAClassifier*) = 0;
	virtual void visit(LinearClassifier*) = 0;
	virtual void visit(LASSOClassifier*) = 0;
	virtual void visit(RandomClassifier*) = 0;
	virtual void visit(NearestNeighborClassifier*) = 0;
	virtual void visit(OwnNearestNeighborClassifier*) = 0;
	virtual void visit(NaiveBayesClassifier*) = 0;
	virtual void visit(BaggingEnsembleClassifier*) = 0;
	virtual void visit(BoostingEnsembleClassifier*) = 0;
};