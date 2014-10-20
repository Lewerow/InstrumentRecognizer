#ifdef BUILD_WITH_SHARK
#include <boost/test/auto_unit_test.hpp>

#include <SharkAdaptor.h>

BOOST_AUTO_TEST_SUITE(Adaptors)
BOOST_AUTO_TEST_SUITE(Classifiers)
BOOST_AUTO_TEST_SUITE(Shark)

BOOST_AUTO_TEST_CASE(IsCallable)
{
	ClassDescriptionBase base;
	base.insert(std::make_pair("One", ClassDescription(2)));
	base.insert(std::make_pair("Two", ClassDescription(3)));

	SharkAdaptor<> t;
	t.setSharkInput(base);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
#endif