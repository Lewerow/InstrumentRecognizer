#include <boost/test/auto_unit_test.hpp>

#include <ClassifierObservers.h>
#include <GeneralClassifierDataObserver.h>

#include "mocks/ReportBuilderMock.hpp"
#include "mocks/ClassifierMock.hpp"
#include "mocks/DescriptionDBManagerMock.hpp"
#include "mocks/RandomServerMock.hpp"
#include "mocks/TimeServerMock.h"

struct ObserverFixture
{
	ObserverFixture() : repBuilder(new ReportBuilderMock), randomServer(new RandomServerMock), timeServer(new TimeServerMock)
	{
		createTestData();
	}
	
	void createTestData()
	{
		ObjectDescription objDesc11;
		objDesc11.push_back(Descriptor(100));
		objDesc11.push_back(Descriptor(202));
		objDesc11.push_back(Descriptor(303));;
		ObjectDescription objDesc12;
		objDesc11.push_back(Descriptor(102));
		objDesc11.push_back(Descriptor(201));
		objDesc11.push_back(Descriptor(305));;
		ObjectDescription objDesc13;
		objDesc11.push_back(Descriptor(150));
		objDesc11.push_back(Descriptor(250));
		objDesc11.push_back(Descriptor(309));;
		ClassDescription clsDesc1;
		clsDesc1.push_back(objDesc11);
		clsDesc1.push_back(objDesc12);
		clsDesc1.push_back(objDesc13);
		
		ObjectDescription objDesc21;
		objDesc21.push_back(Descriptor(-101));
		objDesc21.push_back(Descriptor(-202));
		objDesc21.push_back(Descriptor(-303));
		ObjectDescription objDesc22;
		objDesc21.push_back(Descriptor(-102));
		objDesc21.push_back(Descriptor(-203));
		objDesc21.push_back(Descriptor(-301));
		ObjectDescription objDesc23;
		objDesc21.push_back(Descriptor(-105));
		objDesc21.push_back(Descriptor(-205));
		objDesc21.push_back(Descriptor(-305));
		ClassDescription clsDesc2;
		clsDesc2.push_back(objDesc21);
		clsDesc2.push_back(objDesc22);
		clsDesc2.push_back(objDesc23);
		
		testData.insert(std::make_pair(ClassName("one"), clsDesc1));
		testData.insert(std::make_pair(ClassName("two"), clsDesc2));
	}
	
	DescriptionDBManagerMock descDB;
	ClassifierMock classifier;
	ClassDescriptionBase testData;
	std::shared_ptr<ReportBuilderMock> repBuilder;
	std::shared_ptr<RandomServerMock> randomServer;
	std::shared_ptr<TimeServerMock> timeServer;
};

BOOST_AUTO_TEST_SUITE(Classifiers)
BOOST_FIXTURE_TEST_SUITE(Observers, ObserverFixture)
BOOST_AUTO_TEST_SUITE(DataCutter)

BOOST_AUTO_TEST_CASE(CutsCorrectAmountAndNotifiesReportBuilder)
{
	BOOST_REQUIRE(!testData.empty());
	double cutRatio = 0.5;
	BOOST_REQUIRE(cutRatio < 1);
	
	DataCutterClassifierObserver cutter(cutRatio, randomServer);
	cutter.setReportBuilder(repBuilder);
	
	cutter.notifyStartedTraining(&classifier);
	cutter.notifyIteration(&classifier, 1);
	cutter.notifyFinishedTraining(&classifier);
	mock::verify();

	MOCK_EXPECT(repBuilder->addTotalTrainingObjectCount).once().with(&classifier, unsigned int(3));
	MOCK_EXPECT(repBuilder->addTrainingObjectCount).once().with(&classifier, "one", mock::less_equal(std::size_t(2)));
	MOCK_EXPECT(repBuilder->addTrainingObjectCount).once().with(&classifier, "two", mock::less_equal(std::size_t(2)));
	
	ClassDescriptionBase base(testData);
	
	MOCK_EXPECT(randomServer->generate_uint_1).once().with(std::size_t(1)).returns(1);
	MOCK_EXPECT(randomServer->generate_uint_1).with(mock::any).returns(1);
	cutter.notifyReceivedNewData(&classifier, base);
	
	BOOST_CHECK_EQUAL(base.size(), testData.size());
	BOOST_CHECK(base.at("one").size() >= 1 && base.at("one").size() <= 2);
	BOOST_CHECK(base.at("two").size() >= 1 && base.at("two").size() <= 2);
	BOOST_CHECK_EQUAL(base.at("one").size() + base.at("two").size(), std::size_t(3));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GeneralData)

BOOST_AUTO_TEST_CASE(AppendsOnTheEnd)
{
	GeneralDataClassifierObserver obs;
	obs.setReportBuilder(repBuilder);

	obs.notifyStartedTraining(&classifier);
	obs.notifyIteration(&classifier, 1);
	obs.notifyReceivedNewData(&classifier, testData);
	mock::verify();

	MOCK_EXPECT(repBuilder->addDate).once();
	MOCK_EXPECT(repBuilder->addTitle).once();
	MOCK_EXPECT(repBuilder->addHeader).once();
	MOCK_EXPECT(repBuilder->addFooter).once();

	obs.notifyFinishedTraining(&classifier);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Boundaries)

BOOST_AUTO_TEST_CASE(FinishesAtGivenIterations)
{
	std::chrono::time_point<std::chrono::system_clock> zeroTime;
	BoundaryClassifierObserver obs(timeServer);
	obs.setReportBuilder(repBuilder);
	obs.setMaxIterations(2);
	
	MOCK_EXPECT(timeServer->getCurrentTime).once().returns(zeroTime);
	obs.notifyStartedTraining(&classifier);
	
	obs.notifyIteration(&classifier, 1);
	mock::verify();
	
	MOCK_EXPECT(classifier.doStop).once();
	obs.notifyIteration(&classifier, 3);

	MOCK_EXPECT(timeServer->getCurrentTime).once().returns(zeroTime);
	MOCK_EXPECT(repBuilder->addNumberOfIterations).once().with(&classifier, 3);
	MOCK_EXPECT(repBuilder->addTrainingDuration).once().with(&classifier, zeroTime - zeroTime);
	obs.notifyFinishedTraining(&classifier);
}

BOOST_AUTO_TEST_CASE(FinishesAtGivenTime)
{	
	std::chrono::time_point<std::chrono::system_clock> zeroTime(std::chrono::system_clock::now());
	std::chrono::time_point<std::chrono::system_clock> continueTime(zeroTime + std::chrono::system_clock::duration(10));
	std::chrono::time_point<std::chrono::system_clock> finishTime(zeroTime + std::chrono::system_clock::duration(11));

	BoundaryClassifierObserver obs(timeServer);
	obs.setReportBuilder(repBuilder);
	obs.setMaxDuration(std::chrono::system_clock::duration(10));

	MOCK_EXPECT(timeServer->getCurrentTime).once().returns(zeroTime);
	obs.notifyStartedTraining(&classifier);
	mock::verify();

	MOCK_EXPECT(timeServer->getCurrentTime).once().returns(continueTime);
	obs.notifyIteration(&classifier, 1);
	mock::verify();

	MOCK_EXPECT(classifier.doStop).once();
	MOCK_EXPECT(timeServer->getCurrentTime).once().returns(finishTime);
	obs.notifyIteration(&classifier, 3);
	mock::verify();

	MOCK_EXPECT(timeServer->getCurrentTime).once().returns(finishTime);
	MOCK_EXPECT(repBuilder->addNumberOfIterations).once().with(&classifier, 3);
	MOCK_EXPECT(repBuilder->addTrainingDuration).once().with(&classifier, finishTime - zeroTime);
	obs.notifyFinishedTraining(&classifier);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()