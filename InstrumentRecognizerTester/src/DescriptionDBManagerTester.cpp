#include <boost/test/auto_unit_test.hpp>
#include <DescriptionDBManager.h>

struct FileDescriptionDBManagerFixture
{
	
	boost::filesystem::path path;
	FileDescriptionDBManager db;

	FileDescriptionDBManagerFixture() : path(std::string("../../../InstrumentRecognizerTester/test_files/descriptions")), db(path, 1) {}

};

struct StartedFileDescriptionDBManagerFixture : public FileDescriptionDBManagerFixture
{
	StartedFileDescriptionDBManagerFixture() : MAX_PERCENTAGE_DIFFERENCE(0.01), FileDescriptionDBManagerFixture()
	{
		db.startGathering();
	}

	const double MAX_PERCENTAGE_DIFFERENCE;
};


BOOST_AUTO_TEST_SUITE(DBManagers)
BOOST_AUTO_TEST_SUITE(FileDescription)

BOOST_AUTO_TEST_CASE(WorksWithCrossvalidation)
{
	FileDescriptionDBManager db(("../../../InstrumentRecognizerTester/test_files/descriptions"), 2);
	db.loadDescriptions(boost::filesystem::path("../../../InstrumentRecognizerTester/test_files/descriptions"));

	ClassDescriptionBase test = db.getTestDescriptions();
	ClassDescriptionBase train = db.getTrainingDescriptions();

	BOOST_REQUIRE_EQUAL(2, test.size());
	BOOST_CHECK_EQUAL(1, test.begin()->second.size());
	BOOST_CHECK_EQUAL(1, ((++test.begin())->second.size()));
	BOOST_REQUIRE_EQUAL(1, train.size());
	BOOST_CHECK_EQUAL(1, train.size());
}

BOOST_AUTO_TEST_CASE(WheGivenFileNameAsPathAsserts)
{
	BOOST_CHECK_THROW(FileDescriptionDBManager("../../../InstrumentRecognizerTester/test_files/two/one.txt", 1), AssertException);
}

BOOST_FIXTURE_TEST_CASE(NotStartedFileDescriptionDBManagerThrowsNotStartedWhenAdding, FileDescriptionDBManagerFixture)
{
	BOOST_CHECK_THROW(db.addDescription("x", ObjectDescription()), AssertException);
}

BOOST_FIXTURE_TEST_CASE(NotStartedFileDescriptionDBManagerAssertsWhenFinishing, FileDescriptionDBManagerFixture)
{
	BOOST_CHECK_THROW(db.finishGathering(), AssertException);
}

BOOST_FIXTURE_TEST_CASE(EmptyObjectDescriptionThrowsInvalidNumberOfDescriptors, StartedFileDescriptionDBManagerFixture)
{
	BOOST_CHECK_THROW(db.addDescription("className", ObjectDescription()), FileDescriptionDBManager::InvalidNumberOfDescriptors);
}

BOOST_FIXTURE_TEST_CASE(AfterSingleAddOneElementInResultAfterResetGetsEmpty, StartedFileDescriptionDBManagerFixture)
{
	db.addDescription("className", ObjectDescription(2));
	
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().size(), 1);

	db.reset();
	
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(AddingDifferentNumberOfDescriptorsThrowsInvalidNumberOfDescriptors, StartedFileDescriptionDBManagerFixture)
{
	db.addDescription("className", ObjectDescription(2));
	BOOST_CHECK_THROW(db.addDescription("className", ObjectDescription(4)), FileDescriptionDBManager::InvalidNumberOfDescriptors);
}

BOOST_FIXTURE_TEST_CASE(AfterTwoAddsToSameClassOneClassWithTwoDescriptionsInResult, StartedFileDescriptionDBManagerFixture)
{
	db.addDescription("className", ObjectDescription(2));
	db.addDescription("className", ObjectDescription(2));
	
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().size(), 1);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().at("className").size(), 2);
}

BOOST_FIXTURE_TEST_CASE(AfterTwoAddsToDifferentClassesTwoClassesWithOneDescriptionEachInResult, StartedFileDescriptionDBManagerFixture)
{
	db.addDescription("className1", ObjectDescription(2));
	db.addDescription("className2", ObjectDescription(2));
	
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().size(), 2);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().at("className1").size(), 1);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().at("className2").size(), 1);
}

BOOST_AUTO_TEST_CASE(AddingAfterFinishedAsserts)
{
	FileDescriptionDBManager db("../../../InstrumentRecognizerTester/test_files/trash", 1);
	db.startGathering();

	db.addDescription("className_trash", ObjectDescription(2));
	
	db.finishGathering();

	BOOST_CHECK_THROW(db.addDescription("className2", ObjectDescription(2)), AssertException);

	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().size(), 1);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().count("className_trash"), 1);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().at("className_trash").size(), 1);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().count("className2"), 0);
}

BOOST_FIXTURE_TEST_CASE(DescriptorValuesArePreserved, StartedFileDescriptionDBManagerFixture)
{
	ObjectDescription class1Desc1(4);
//	class1Desc1[0] = Descriptor(3, 2);
//	class1Desc1[1] = Descriptor(-4, 1);
//	class1Desc1[2] = Descriptor(3, 0.5);
//	class1Desc1[3] = Descriptor(2, 2);
	class1Desc1[0] = Descriptor(3);
	class1Desc1[1] = Descriptor(-4);
	class1Desc1[2] = Descriptor(3);
	class1Desc1[3] = Descriptor(2);

	ObjectDescription class1Desc2(4);
//	class1Desc2[0] = Descriptor(4, 20);
//	class1Desc2[1] = Descriptor(14, 51);
//	class1Desc2[2] = Descriptor(3.55, 0.5);
//	class1Desc2[3] = Descriptor(2.1, 2);
	class1Desc2[0] = Descriptor(20);
	class1Desc2[1] = Descriptor(51);
	class1Desc2[2] = Descriptor(0.5);
	class1Desc2[3] = Descriptor(2);

	ObjectDescription class2Desc(4);
//	class2Desc[0] = Descriptor(-1, 6);
//	class2Desc[1] = Descriptor(-11, 6.11);
//	class2Desc[2] = Descriptor(7, 999);
//	class2Desc[3] = Descriptor(-1, 1);
	class2Desc[0] = Descriptor(6);
	class2Desc[1] = Descriptor(6.11);
	class2Desc[2] = Descriptor(999);
	class2Desc[3] = Descriptor(1);

	db.addDescription("className1", class1Desc1);
	db.addDescription("className1", class1Desc2);
	db.addDescription("className2", class2Desc);

	const ClassDescriptionBase& base = db.getTrainingDescriptions();

	for(std::size_t i = 0; i < class1Desc1.size(); i++)
	{
/*		BOOST_CHECK_CLOSE(base.at("className1").at(0).at(i).real(), class1Desc1.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(0).at(i).imag(), class1Desc1.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(1).at(i).real(), class1Desc2.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(1).at(i).imag(), class1Desc2.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className2").at(0).at(i).real(), class2Desc.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className2").at(0).at(i).imag(), class2Desc.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
*/
		BOOST_CHECK_CLOSE(base.at("className1").at(0).at(i), class1Desc1.at(i), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(1).at(i), class1Desc2.at(i), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className2").at(0).at(i), class2Desc.at(i), MAX_PERCENTAGE_DIFFERENCE);

	}
}

BOOST_FIXTURE_TEST_CASE(FinishingSavesEachClassToFileInReadableForm, StartedFileDescriptionDBManagerFixture)
{

	ObjectDescription class1Desc1(4);
	//	class1Desc1[0] = Descriptor(3, 2);
	//	class1Desc1[1] = Descriptor(-4, 1);
	//	class1Desc1[2] = Descriptor(3, 0.5);
	//	class1Desc1[3] = Descriptor(2, 2);
		class1Desc1[0] = Descriptor(3);
		class1Desc1[1] = Descriptor(-4);
		class1Desc1[2] = Descriptor(3);
		class1Desc1[3] = Descriptor(2);

	ObjectDescription class1Desc2(4);
	//	class1Desc2[0] = Descriptor(4, 20);
	//	class1Desc2[1] = Descriptor(14, 51);
	//	class1Desc2[2] = Descriptor(3.55, 0.5);
	//	class1Desc2[3] = Descriptor(2.1, 2);
	class1Desc2[0] = Descriptor(20);
	class1Desc2[1] = Descriptor(51);
	class1Desc2[2] = Descriptor(0.5);
	class1Desc2[3] = Descriptor(2);

	ObjectDescription class2Desc(4);
	//	class2Desc[0] = Descriptor(-1, 6);
	//	class2Desc[1] = Descriptor(-11, 6.11);
	//	class2Desc[2] = Descriptor(7, 999);
	//	class2Desc[3] = Descriptor(-1, 1);
	class2Desc[0] = Descriptor(6);
	class2Desc[1] = Descriptor(6.11);
	class2Desc[2] = Descriptor(7);
	class2Desc[3] = Descriptor(1);

	db.addDescription("className1", class1Desc1);
	db.addDescription("className1", class1Desc2);
	db.addDescription("className2", class2Desc);

	db.finishGathering();
	
	BOOST_REQUIRE_EQUAL(db.getTrainingDescriptions().size(), 2);

	db.reset();
	
	BOOST_REQUIRE_EQUAL(db.getTrainingDescriptions().size(), 0);
	
	db.loadDescriptions(path);
	
	BOOST_REQUIRE_EQUAL(db.getTrainingDescriptions().size(), 2);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().at("className1").size(), 2);
	BOOST_CHECK_EQUAL(db.getTrainingDescriptions().at("className2").size(), 1);


	const ClassDescriptionBase& base = db.getTrainingDescriptions();

	for(std::size_t i = 0; i < class1Desc1.size(); i++)
	{
		/*
		BOOST_CHECK_CLOSE(base.at("className1").at(0).at(i).real(), class1Desc1.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(0).at(i).imag(), class1Desc1.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(1).at(i).real(), class1Desc2.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(1).at(i).imag(), class1Desc2.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className2").at(0).at(i).real(), class2Desc.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className2").at(0).at(i).imag(), class2Desc.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		*/
		BOOST_CHECK_CLOSE(base.at("className1").at(1).at(i), class1Desc2.at(i), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className2").at(0).at(i), class2Desc.at(i), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(base.at("className1").at(0).at(i), class1Desc1.at(i), MAX_PERCENTAGE_DIFFERENCE);
	}
}

BOOST_FIXTURE_TEST_CASE(InvalidFileThrowsInvalidFile, StartedFileDescriptionDBManagerFixture)
{
	BOOST_CHECK_THROW(db.loadDescriptions(boost::filesystem::path("../../../InstrumentRecognizerTester/test_files/invalid_description")), FileDescriptionDBManager::InvalidFile);
}



BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
