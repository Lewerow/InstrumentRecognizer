#include <boost/test/auto_unit_test.hpp>

#include <ClassifierFactory.h>
#include <Classifier.h>

#include <memory>

#include "mocks/ClassifierMock.hpp"

struct ClassifierFactoryFixture
{
	ClassifierFactoryFixture() : builderMock(new ClassifierBuilderMock) {}

	ClassifierFactory factory;
	std::shared_ptr<ClassifierBuilderMock> builderMock;
};

BOOST_AUTO_TEST_SUITE(Factories)
BOOST_FIXTURE_TEST_SUITE(Classifiers, ClassifierFactoryFixture)

BOOST_AUTO_TEST_CASE(NoClassifiersAtTheBeginningOneAfterAddingNoneAfterDeleting)
{
	BOOST_CHECK_EQUAL(factory.availableClassifiers().size(), 0);

	factory.registerBuilder("mock", builderMock);
	BOOST_CHECK_EQUAL(factory.availableClassifiers().size(), 1);

	factory.deregisterBuilder("mock");
	BOOST_CHECK_EQUAL(factory.availableClassifiers().size(), 0);
}

BOOST_AUTO_TEST_CASE(RegisteredClassifiersAreAvailable)
{
	factory.registerBuilder("bob", builderMock);
	factory.registerBuilder("steve", builderMock);
	factory.registerBuilder("john", builderMock);

	std::unordered_set<ClassifierFactory::RegistrationKey> keys = factory.availableClassifiers();
	
	std::unordered_set<ClassifierFactory::RegistrationKey> keysExpected;
	keysExpected.insert("bob");
	keysExpected.insert("steve");
	keysExpected.insert("john");
	
	BOOST_CHECK_EQUAL(keys.size(), keysExpected.size());

}

BOOST_AUTO_TEST_CASE(RegisteringSameBuilderTwice)
{
	BOOST_CHECK_EQUAL(factory.availableClassifiers().size(), 0);

	factory.registerBuilder("mock", builderMock);
	BOOST_CHECK_EQUAL(factory.availableClassifiers().size(), 1);

	BOOST_CHECK_THROW(factory.registerBuilder("mock", builderMock), AssertException);
	BOOST_CHECK_EQUAL(factory.availableClassifiers().size(), 1);

}

BOOST_AUTO_TEST_CASE(DeregisteringNonExistentBuilder)
{
	BOOST_CHECK_THROW(factory.deregisterBuilder("mock"), AssertException);
}

BOOST_AUTO_TEST_CASE(RegisterDeregisterRegister)
{
	BOOST_CHECK_THROW(factory.deregisterBuilder("mock"), AssertException);
}

BOOST_AUTO_TEST_CASE(RegisteredClassifiersAreUsable)
{
	factory.registerBuilder("mock", builderMock);

	MOCK_EXPECT(builderMock->buildString).once().with(std::string("abc")).returns(new ClassifierMock);
	std::shared_ptr<Classifier> classifierStr(factory.createClassifier("mock", std::string("abc")));

	MOCK_EXPECT(builderMock->buildXMLNode).once().with(XMLNode()).returns(new ClassifierMock);
	std::shared_ptr<Classifier> classifierXMLNode(factory.createClassifier("mock", XMLNode()));
}

BOOST_AUTO_TEST_CASE(DefaultBuilderWorks)
{
	BOOST_CHECK_THROW(factory.setDefaultBuilder("mock"), ClassifierFactory::ClassifierNotRegistered);
	BOOST_CHECK_THROW(factory.createDefaultClassifier(std::string("str")), ClassifierFactory::ClassifierNotRegistered);
	BOOST_CHECK_THROW(factory.createDefaultClassifier(XMLNode()), ClassifierFactory::ClassifierNotRegistered);

	factory.registerBuilder("mock", builderMock);
	factory.setDefaultBuilder("mock");
		
	MOCK_EXPECT(builderMock->buildString).once().with(std::string("abc")).returns(new ClassifierMock);
	std::shared_ptr<Classifier> classifierStr(factory.createDefaultClassifier(std::string("abc")));

	MOCK_EXPECT(builderMock->buildXMLNode).once().with(XMLNode()).returns(new ClassifierMock);
	std::shared_ptr<Classifier> classifierXMLNode(factory.createDefaultClassifier(XMLNode()));
}

BOOST_AUTO_TEST_CASE(ThrowsAtUnregisteredClassifierUse)
{
	BOOST_CHECK_THROW(factory.createClassifier("mock", std::string("abc")), ClassifierFactory::ClassifierNotRegistered);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()