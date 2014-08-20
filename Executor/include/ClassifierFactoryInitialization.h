class ClassifierFactory;

#include <string>
#include <memory>

class RandomServer;

namespace setup
{
	ClassifierFactory* classifier_factory(const std::string& defaultClassifier, std::shared_ptr<RandomServer>);
}