#include <boost/test/auto_unit_test.hpp>

#include <EqualSizeDiscretizer.h>
#include <EqualFrequencyDiscretizer.h>

struct DiscretizerFixture
{
	DiscretizerFixture()
	{
		{
			ClassDescription a{ { 1, 1, 1 }, { 1, 2, 4 }, { 2, 2, 2 } };
			b.insert(std::make_pair("a", a));
		}
		{
			ClassDescription a{ { 6, 6, 5 }, { 6, 5, 8 }, { 5, 7, 8 } };
			b.insert(std::make_pair("b", a));
    	}
		{
			ClassDescription a{ { 2, 10, 16 }, { 1, 21, 14 }, { 0, 12, 32 } };
			b.insert(std::make_pair("c", a));
		}
	}

	ClassDescriptionBase b;
};

BOOST_FIXTURE_TEST_SUITE(Discretizers, DiscretizerFixture)

BOOST_AUTO_TEST_CASE(EqualSizeDiscretizerDiscretizesToEqualSizes)
{
	EqualSizeDiscretizer attr_0(3, b, 0);
	attr_0.teach();

	BOOST_CHECK_EQUAL(attr_0.discretize(0), 0);
	BOOST_CHECK_EQUAL(attr_0.discretize(1), 0);
	BOOST_CHECK_EQUAL(attr_0.discretize(1.9), 0);
	BOOST_CHECK_EQUAL(attr_0.discretize(2.1), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(3.9), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(4.1), 2);
	BOOST_CHECK_EQUAL(attr_0.discretize(4.5), 2);
	BOOST_CHECK_EQUAL(attr_0.discretize(5.9), 2);
	BOOST_CHECK_EQUAL(attr_0.discretize(59), 2);
}

BOOST_AUTO_TEST_CASE(EqualFrequencyDiscretizerDiscretizesToEqualFrequencies)
{
	EqualFrequencyDiscretizer attr_0(3, b, 0);
	attr_0.teach();

	BOOST_CHECK_EQUAL(attr_0.discretize(0), 0);
	BOOST_CHECK_EQUAL(attr_0.discretize(1), 0);
	BOOST_CHECK_EQUAL(attr_0.discretize(1.45), 0);
	BOOST_CHECK_EQUAL(attr_0.discretize(1.55), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(2), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(2.5), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(3.9), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(4.1), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(4.5), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(5), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(5.45), 1);
	BOOST_CHECK_EQUAL(attr_0.discretize(5.55), 2);
	BOOST_CHECK_EQUAL(attr_0.discretize(5.9), 2);
	BOOST_CHECK_EQUAL(attr_0.discretize(59), 2);
}

BOOST_AUTO_TEST_SUITE_END()