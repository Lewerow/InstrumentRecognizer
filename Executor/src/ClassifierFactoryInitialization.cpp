#include "ClassifierFactoryInitialization.h"

#include <ClassifierFactory.h>
#include <Classifier.h>
#include <LinearClassifier.h>
#include <LASSOClassifier.h>
#include <NearestNeighborClassifier.h>
#include <NearestNeighborsStrategies.h>
#include <RandomClassifier.h>

namespace setup
{
	ClassifierFactory* classifier_factory(const std::string& defaultClassifier, std::shared_ptr<RandomServer> randomness)
	{
		ClassifierFactory* factory = new ClassifierFactory;
		factory->registerBuilder("Linear", std::shared_ptr<Classifier::Builder>(new LinearClassifier::Builder));
		factory->registerBuilder("Random", std::shared_ptr<Classifier::Builder>(new RandomClassifier::Builder(randomness)));
		factory->registerBuilder("NearestNeighbor_StandardTree", std::shared_ptr<Classifier::Builder>(new NearestNeighborClassifier::Builder(new StandardTreeStrategy(1))));
		factory->registerBuilder("LASSO", std::shared_ptr<Classifier::Builder>(new LASSOClassifier::Builder()));
	
		auto available = factory->availableClassifiers();

		if (available.count(defaultClassifier) == 0)
			throw std::out_of_range(std::accumulate(available.begin(), available.end(), std::string("Classifier: ") + defaultClassifier + " is not available. Available classifiers: \n",
				[](const std::string& acc, const std::string& app){return acc +"-> " + app + "\n"; }));

		factory->setDefaultBuilder(defaultClassifier);
		return factory;
	}
}