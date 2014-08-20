#include <boost/test/auto_unit_test.hpp>
#include <boost/cast.hpp>

#include "mocks/ClassifierMock.hpp"
#include <ClassifierDBManager.h>
#include <ClassifierFactory.h>

struct ClassifierDBManagerFixture
{
	ClassifierDBManagerFixture() : factory(new ClassifierFactory), classifierMock(new ClassifierMock)
	{
		factory->registerBuilder("Mock", std::shared_ptr<Classifier::Builder>(new ClassifierBuilderMock));
	}

	std::shared_ptr<ClassifierFactory> factory;
	std::shared_ptr<ClassifierMock> classifierMock;
};

BOOST_AUTO_TEST_SUITE(DBManagers)
BOOST_FIXTURE_TEST_SUITE(XmlFileClassifier, ClassifierDBManagerFixture)

BOOST_AUTO_TEST_CASE(AssertsOnNonExistentDir)
{
	BOOST_CHECK_THROW(XmlFileClassifierDBManager("non_existent_dir", factory), AssertException);
}

BOOST_AUTO_TEST_CASE(CannotWriteEmpty)
{
	XmlFileClassifierDBManager mgr("../../../InstrumentRecognizerTester/test_files/classifiers/one", factory);

	// pattern thief!
	ClassifierBuilderMock* builderMock(new ClassifierBuilderMock);
	MOCK_EXPECT(classifierMock->doMakeBuilder).once().returns(builderMock);
	MOCK_EXPECT(builderMock->dismantleToXML).once().with(classifierMock).calls([](std::shared_ptr<Classifier>){return std::unique_ptr<XMLNode>(nullptr); });
	BOOST_CHECK_THROW(mgr.addClassifier(classifierMock, "Mock"), AssertException);
}

BOOST_AUTO_TEST_CASE(SaveAndLoadCallsProperBuilders)
{
	XmlFileClassifierDBManager mgr("../../../InstrumentRecognizerTester/test_files/classifiers/one", factory);

	// pattern thief!
	ClassifierBuilderMock* builderMock(new ClassifierBuilderMock);
	MOCK_EXPECT(classifierMock->doMakeBuilder).once().returns(builderMock);
	MOCK_EXPECT(builderMock->dismantleToXML).once().with(classifierMock).calls([](std::shared_ptr<Classifier>){return std::unique_ptr<XMLNode>(new XMLNode); });
	mgr.addClassifier(classifierMock, "Mock");
	BOOST_REQUIRE(mock::verify());
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()