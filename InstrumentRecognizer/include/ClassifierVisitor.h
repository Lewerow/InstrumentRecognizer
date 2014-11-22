class Classifier;
class LinearClassifier;
class RandomClassifier;
class NearestNeighborClassifier;
class LASSOClassifier;
class ILAClassifier;
class NaiveBayesClassifier;

class ClassifierVisitor
{
public:
	virtual void visit(Classifier*) = 0;
	virtual void visit(ILAClassifier*) = 0;
	virtual void visit(LinearClassifier*) = 0;
	virtual void visit(LASSOClassifier*) = 0;
	virtual void visit(RandomClassifier*) = 0;
	virtual void visit(NearestNeighborClassifier*) = 0;
	virtual void visit(NaiveBayesClassifier*) = 0;
};