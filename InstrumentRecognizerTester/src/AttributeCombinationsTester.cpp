#include <boost/test/auto_unit_test.hpp>

#include <AttributeCombinations.h>

namespace
{
    void compareVector(const std::vector<unsigned int>& expected, const std::vector<unsigned int>& actual)
    {
        BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), actual.begin(), actual.end());
    }
}

BOOST_AUTO_TEST_SUITE(Classifiers)

BOOST_AUTO_TEST_CASE(AttributeCombintionsTests)
{
    AttributeCombinations attributes(2);
    compareVector({ 0 }, attributes.getNextAttributeSet());
    compareVector({ 1 }, attributes.getNextAttributeSet());
    compareVector({ 0, 1 }, attributes.getNextAttributeSet());
    compareVector({}, attributes.getNextAttributeSet());
}

BOOST_AUTO_TEST_CASE(AttributeCombintionsTestsForFourAttributes)
{
    AttributeCombinations attributes(4);
    compareVector({ 0 }, attributes.getNextAttributeSet());
    compareVector({ 1 }, attributes.getNextAttributeSet());
    compareVector({ 2 }, attributes.getNextAttributeSet());
    compareVector({ 3 }, attributes.getNextAttributeSet());
    compareVector({ 0, 1 }, attributes.getNextAttributeSet());
    compareVector({ 0, 2 }, attributes.getNextAttributeSet());
    compareVector({ 0, 3 }, attributes.getNextAttributeSet());
    compareVector({ 1, 2 }, attributes.getNextAttributeSet());
    compareVector({ 1, 3 }, attributes.getNextAttributeSet());
    compareVector({ 2, 3 }, attributes.getNextAttributeSet());
    compareVector({ 0, 1, 2}, attributes.getNextAttributeSet());
    compareVector({ 0, 1, 3 }, attributes.getNextAttributeSet());
    compareVector({ 0, 2, 3 }, attributes.getNextAttributeSet());
    compareVector({ 1, 2, 3 }, attributes.getNextAttributeSet());
    compareVector({ 0, 1, 2, 3 }, attributes.getNextAttributeSet());
    compareVector({}, attributes.getNextAttributeSet());
    compareVector({}, attributes.getNextAttributeSet());
    compareVector({}, attributes.getNextAttributeSet());
    compareVector({}, attributes.getNextAttributeSet());
}

BOOST_AUTO_TEST_SUITE_END()