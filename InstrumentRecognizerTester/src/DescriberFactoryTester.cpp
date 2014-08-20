#include <boost/test/auto_unit_test.hpp>

#include "mocks/DescriberMock.hpp"

#include <DescriberFactory.h>

BOOST_AUTO_TEST_SUITE(Factories)
BOOST_AUTO_TEST_SUITE(Describers)

BOOST_AUTO_TEST_CASE(DescriberFactoryCanConstructDescribers)
{
	std::shared_ptr<DescriberMock> describerMock(new DescriberMock);
	DescriberFactory factory(describerMock);

	MOCK_EXPECT(describerMock->clone).once().returns(describerMock.get());

	factory.getDescriber();
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()