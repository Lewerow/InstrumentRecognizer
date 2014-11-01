#include <boost/test/auto_unit_test.hpp>

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
    ILARule rule(std::string("a"), { boost::none, boost::none, 2 });

    BOOST_CHECK_EQUAL(boost::none, rule.classify({ 1, 1, 1 }));
    BOOST_CHECK_EQUAL(std::string("a"), rule.classify({ 1, 2, 2 }));
}

BOOST_AUTO_TEST_CASE(ILARuleRefinedTests)
{
    ILARule rule(std::string("a"), { 3, boost::none, 2, boost::none });

    BOOST_CHECK_EQUAL(boost::none, rule.classify({ 3, 1, 1, 3 }));
    BOOST_CHECK_EQUAL(boost::none, rule.classify({ 1, 2, 2, 2 }));
    BOOST_CHECK_EQUAL(std::string("a"), rule.classify({ 3, 3, 2, 2 }));
}

BOOST_AUTO_TEST_CASE(ILAClassifierBasicTests)
{
    ILAClassifier classifier;
    classifier.setInputData(getBasicClassDescriptionBase());
    classifier.run();

    BOOST_CHECK_EQUAL("", classifier.classify({ 1, 10, 100 }));
//    BOOST_CHECK_EQUAL("a", classifier.classify({ 1, 1, 1 }));
//    BOOST_CHECK_EQUAL("b", classifier.classify({ 11, 12, 13 }));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()