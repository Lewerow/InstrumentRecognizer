#include <boost/test/auto_unit_test.hpp>

#include <NaiveBayesClassifier.h>
#include <EqualSizeDiscretizer.h>

#include "ExampleClassDescriptions.hpp"

BOOST_AUTO_TEST_SUITE(Classifiers)
BOOST_AUTO_TEST_SUITE(NaiveBayes)

BOOST_AUTO_TEST_CASE(WorksProperly)
{
	std::shared_ptr<DiscretizerFactory> factory(std::make_shared<DiscretizerFactory>(10));
	factory->registerCreator("default", EqualSizeDiscretizer::creator);
	factory->setDefault("default");

	NaiveBayesClassifier classifier(factory);
	classifier.setInputData(getBasicClassDescriptionBase());
	classifier.run();

	BOOST_CHECK_EQUAL("a", classifier.classify({ 1, 1, 1 }));
	BOOST_CHECK_EQUAL("b", classifier.classify({ 11, 12, 13 }));
	BOOST_CHECK_EQUAL("c", classifier.classify({ 21, 22, 13 }));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()