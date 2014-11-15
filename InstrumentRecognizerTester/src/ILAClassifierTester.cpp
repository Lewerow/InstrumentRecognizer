#include <boost/test/auto_unit_test.hpp>

#include <EqualSizeDiscretizer.h>
#include <ILAClassifier.h>
#include <ILARule.h>

ClassDescriptionBase getBasicClassDescriptionBase()
{
    ClassDescriptionBase base;
    ClassDescription a({ { 1, 2, 3 }, { 2, 3, 4 }, { 1.5, 2.5, 3.5 } });
    ClassDescription b({ { 10, 10, 10 }, { 12, 12, 12 }, { 11, 11, 11 } });
    ClassDescription c({ { 20, 22, 23 }, { 21, 21, 21 }, { 23.1, 23.4, 23.1 } });

    base.insert(std::make_pair("a", a));
    base.insert(std::make_pair("b", b));
    base.insert(std::make_pair("c", c));

    return base;
}

BOOST_AUTO_TEST_SUITE(Classifiers)
BOOST_AUTO_TEST_SUITE(ILA)

BOOST_AUTO_TEST_CASE(ILARuleTests)
{
    ILARule rule(std::string("a"), { {2, 2} });

	BOOST_CHECK_EQUAL(false, rule.matches({ 1, 1, 1 }));
	BOOST_CHECK_EQUAL(true, rule.matches({ 1, 2, 2 }));
	BOOST_CHECK_EQUAL("a", rule.name());
}

BOOST_AUTO_TEST_CASE(ILARuleRefinedTests)
{
    ILARule rule(std::string("a"), { {0, 3}, {2,2} });

    BOOST_CHECK_EQUAL(false, rule.matches({ 3, 1, 1, 3 }));
    BOOST_CHECK_EQUAL(false, rule.matches({ 1, 2, 2, 2 }));
	BOOST_CHECK_EQUAL(true, rule.matches({ 3, 3, 2, 2 }));
	BOOST_CHECK_EQUAL("a", rule.name());
}

BOOST_AUTO_TEST_CASE(ILAClassifierBasicTests)
{
	std::shared_ptr<DiscretizerFactory> factory(std::make_shared<DiscretizerFactory>(10));
	factory->registerCreator("default", EqualSizeDiscretizer::creator);
	factory->setDefault("default");

    ILAClassifier classifier(factory);
    classifier.setInputData(getBasicClassDescriptionBase());
    classifier.run();

    BOOST_CHECK_EQUAL("", classifier.classify({ 5, 10, 100 }));
    BOOST_CHECK_EQUAL("a", classifier.classify({ 1, 1, 1 }));
    BOOST_CHECK_EQUAL("b", classifier.classify({ 11, 12, 13 }));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()