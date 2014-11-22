#include <boost/test/auto_unit_test.hpp>
#include <boost/lexical_cast.hpp>

#include <ClassifierOverlord.h>

#include "mocks/DescriptionDBManagerMock.hpp"
#include "mocks/ClassifierDBManagerMock.hpp"
#include "mocks/ClassifierFactoryMock.hpp"
#include "mocks/ReportBuilderMock.hpp"
#include "mocks/ClassifierObserverMock.hpp"
#include "mocks/ClassifierMock.hpp"

struct ClassifierOverlordFixture
{
	ClassifierOverlordFixture() : classifierFactoryMock(new ClassifierFactoryMock), classifierDBManagerMock(new ClassifierDBManagerMock), descriptionDBManagerMock(new DescriptionDBManagerMock), 
		reportBuilderMock(new ReportBuilderMock), classifierMock(new ClassifierMock), overlord(classifierFactoryMock, classifierDBManagerMock, descriptionDBManagerMock)
	{
		overlord.setReportBuilder(reportBuilderMock);
		observerMocks.push_back(std::shared_ptr<ClassifierObserverMock>(new ClassifierObserverMock));
		observerMocks.push_back(std::shared_ptr<ClassifierObserverMock>(new ClassifierObserverMock));
		
		// Liskov substitution hack
		overlord.setObservers(std::vector<std::shared_ptr<ClassifierObserver> > (observerMocks.begin(), observerMocks.end()));
	}
	
	std::shared_ptr<ClassifierFactoryMock> classifierFactoryMock;
	std::shared_ptr<ClassifierDBManagerMock> classifierDBManagerMock;
	std::shared_ptr<DescriptionDBManagerMock> descriptionDBManagerMock;
	std::shared_ptr<ReportBuilderMock> reportBuilderMock;
	std::shared_ptr<ClassifierMock> classifierMock;
	std::vector<std::shared_ptr<ClassifierObserverMock> > observerMocks;

	ClassifierOverlord overlord;
};

BOOST_AUTO_TEST_SUITE(Classifiers)
BOOST_FIXTURE_TEST_SUITE(Overlord, ClassifierOverlordFixture)

BOOST_AUTO_TEST_CASE(WorksCorrectly)
{
	Report::ID reportID(10);

	MOCK_EXPECT(descriptionDBManagerMock->areFoldsRemaining).once().returns(true);

	MOCK_EXPECT(classifierFactoryMock->createDefaultClassifierEmpty).once().returns(classifierMock);
	for(auto& o: observerMocks)
		MOCK_EXPECT(o->setReportBuilder).once().with(reportBuilderMock);
	
	MOCK_EXPECT(reportBuilderMock->startReport).once().with(classifierMock.get()).returns(reportID);
		
	ClassDescriptionBase base;
	MOCK_EXPECT(descriptionDBManagerMock->getTrainingDescriptions).once().returns(base);
	for (auto& o : observerMocks)
		MOCK_EXPECT(o->notifyReceivedNewData).once();

	MOCK_EXPECT(classifierMock->doSetInputData).once().with(base);
	for (auto& o : observerMocks)
		MOCK_EXPECT(o->notifyStartedTraining).once();
	
    mock::sequence seq;
	MOCK_EXPECT(classifierMock->doRun).once().in(seq);
    MOCK_EXPECT(classifierMock->doStop).once().in(seq);
    for (auto& o : observerMocks)
        MOCK_EXPECT(o->notifyFinishedTraining).once().with(classifierMock.get());

    MOCK_EXPECT(reportBuilderMock->endReportClassifier).once().with(classifierMock.get()).in(seq);
	
	MOCK_EXPECT(classifierDBManagerMock->addClassifier).once().with(classifierMock, boost::lexical_cast<std::string>(reportID));

	MOCK_EXPECT(descriptionDBManagerMock->areFoldsRemaining).once().returns(false);
    MOCK_EXPECT(descriptionDBManagerMock->nextCrossValidationSet).once();

    MOCK_EXPECT(reportBuilderMock->summarize).once();
    
	overlord.teachOne();
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()