#include <boost/test/auto_unit_test.hpp>

#include "mocks/DataDBManagerMock.hpp"
#include "mocks/DescriptionDBManagerMock.hpp"
#include "mocks/DescriberFactoryMock.hpp"
#include "mocks/DescriberMock.hpp"

#include <DescriberOverlord.h>

#include <sstream>
#include <memory>
#include <functional>

namespace s = std::placeholders;

namespace
{
	void checkStreamsEqual(std::istream* lhs, std::istream* rhs)
	{		
		std::string lhsStr((std::istreambuf_iterator<char>(*lhs)), std::istreambuf_iterator<char>());
		std::string rhsStr((std::istreambuf_iterator<char>(*rhs)), std::istreambuf_iterator<char>());

		BOOST_CHECK_EQUAL(lhsStr, rhsStr);
	}
}

struct DescriberOverlordFixture
{
	DescriberOverlordFixture() : dataDBManager(new DataDBManagerMock), descriptionDBManager(new DescriptionDBManagerMock), describerFactory(new DescriberFactoryMock),
		overlord(dataDBManager, descriptionDBManager, describerFactory)
	{}

	std::shared_ptr<DataDBManagerMock> dataDBManager;
	std::shared_ptr<DescriptionDBManagerMock> descriptionDBManager;
	std::shared_ptr<DescriberFactoryMock> describerFactory;
	DescriberOverlord overlord;
};

BOOST_AUTO_TEST_SUITE(Describers)
BOOST_FIXTURE_TEST_SUITE(Overlord, DescriberOverlordFixture)

BOOST_AUTO_TEST_CASE(CanProcessData)
{
	mock::sequence seq, seq2;

	MOCK_EXPECT(descriptionDBManager->startGathering).once().in(seq);


	ObjectDescription objDesc(4);
	std::stringstream inputStream;
	MOCK_EXPECT(dataDBManager->areRecordsAvailable).once().in(seq).in(seq2).returns(true);
	MOCK_EXPECT(dataDBManager->areRecordsAvailable).once().in(seq2).returns(false);

	MOCK_EXPECT(dataDBManager->take).once().in(seq).returns(DataRecord("test", inputStream, "className"));

	// pattern "Thief" -> mock on factory
	DescriberMock* describerMock = new DescriberMock;
	MOCK_EXPECT(describerFactory->getDescriber).once().in(seq).returns(describerMock);
	MOCK_EXPECT(describerMock->setInputStream).once().in(seq).calls(std::bind(checkStreamsEqual, s::_1, &inputStream));
	MOCK_EXPECT(describerMock->generateObjectDescription).once().in(seq).returns(objDesc);
	MOCK_EXPECT(descriptionDBManager->addDescription).once().in(seq).with("className", objDesc);
	MOCK_EXPECT(dataDBManager->release).once().in(seq).with("test").in(seq);
	MOCK_EXPECT(descriptionDBManager->finishGathering).once().in(seq);
		
	overlord.startProcessing();	
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()