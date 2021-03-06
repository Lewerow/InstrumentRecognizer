#include <boost/test/unit_test.hpp>

#include <DataDBManager.h>

#include <boost/algorithm/string.hpp>

std::string readEntireRecord(DataRecord record)
{
	std::stringstream buffer;
	buffer << record.input.rdbuf();
	return buffer.str();
}

BOOST_AUTO_TEST_SUITE(DBManagers)

BOOST_AUTO_TEST_SUITE(LineData)

BOOST_AUTO_TEST_CASE(LoadsProperDataCount)
{
    std::stringstream s;
    s << "1;1;1;jajko" << std::endl;
    s << "2;2;2;zupa" << std::endl;

    LineDataDBManager mgr(s, 3);

    auto rec1 = mgr.take();
    BOOST_CHECK_EQUAL("jajko", rec1.className);

    auto rec2 = mgr.take();
    BOOST_CHECK_EQUAL("zupa", rec2.className);

    BOOST_CHECK(!mgr.areRecordsAvailable());
    BOOST_CHECK_THROW(mgr.take(), DataDBManager::NoMoreRecordsException);

    mgr.release(rec1.dbKey);
    mgr.release(rec2.dbKey);
}

BOOST_AUTO_TEST_CASE(LoadsProperDataCountWithOtherOrder)
{
    std::stringstream s;
    s << "jajko;1;1;1" << std::endl;
    s << "zupa;2;2;2" << std::endl;
    s << "okno;4.3;4.4;4.1" << std::endl;

    LineDataDBManager mgr(s, 0);

    char f;
    double a, b, c;

    auto rec1 = mgr.take();
    rec1.input >> a >> f >> b >> f >> c;
    BOOST_CHECK_EQUAL("jajko", rec1.className);
    BOOST_CHECK_CLOSE(1, a, 0.1);
    BOOST_CHECK_CLOSE(1, b, 0.1);
    BOOST_CHECK_CLOSE(1, c, 0.1);

    auto rec2 = mgr.take();
    rec2.input >> a >> f >> b >> f >> c;
    BOOST_CHECK_EQUAL("zupa", rec2.className);
    BOOST_CHECK_CLOSE(2, a, 0.1);
    BOOST_CHECK_CLOSE(2, b, 0.1);
    BOOST_CHECK_CLOSE(2, c, 0.1);

    auto rec3 = mgr.take();
    rec3.input >> a >> f >> b >> f >> c;
    BOOST_CHECK_EQUAL("okno", rec3.className);
    BOOST_CHECK_CLOSE(4.3, a, 0.1);
    BOOST_CHECK_CLOSE(4.4, b, 0.1);
    BOOST_CHECK_CLOSE(4.1, c, 0.1);

    BOOST_CHECK(!mgr.areRecordsAvailable());
    BOOST_CHECK_THROW(mgr.take(), DataDBManager::NoMoreRecordsException);

    mgr.release(rec1.dbKey);
    mgr.release(rec2.dbKey);
    mgr.release(rec3.dbKey);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(FileData)

BOOST_AUTO_TEST_CASE(DirThatDoesNotExist)
{
	BOOST_CHECK_THROW(FileDataDBManager dbManager(boost::filesystem::current_path().string() + "\\non_existent_dir", ".txt"), DataDBManager::InvalidPathException);
}

BOOST_AUTO_TEST_CASE(ReleaseNonExistentTrickyDir)
{
	FileDataDBManager dbManager("../../../InstrumentRecognizerTester/test_files/tricky_dir", ".txt");

	BOOST_CHECK_THROW(dbManager.release("0"), AssertException);
}

BOOST_AUTO_TEST_CASE(ClassNameIsFirstPreLastPartOfName)
{
	FileDataDBManager dbManagerOne("../../../InstrumentRecognizerTester/test_files/tree_root/one", ".txt");
	
	while(dbManagerOne.areRecordsAvailable())
	{
		DataRecord record = dbManagerOne.take();
		BOOST_CHECK_EQUAL(record.className, "one");
		dbManagerOne.release(record.dbKey);
	}

	FileDataDBManager dbManagerTricky("../../../InstrumentRecognizerTester/test_files/tree_root/tricky_dir", ".txt");
	while(dbManagerTricky.areRecordsAvailable())
	{
		DataRecord record = dbManagerTricky.take();
		BOOST_CHECK_EQUAL(record.className, "tricky_dir");
		dbManagerTricky.release(record.dbKey);
	}
	
}

BOOST_AUTO_TEST_CASE(GetThreeReleaseThreeTrickyDir)
{
	FileDataDBManager dbManager("../../../InstrumentRecognizerTester/test_files/tricky_dir", ".txt");

	while(dbManager.areRecordsAvailable())
	{
		DataRecord record = dbManager.take();
		dbManager.release(record.dbKey);
	}
	
	BOOST_CHECK_EQUAL(dbManager.filesInBufferCount(), 0);
	BOOST_CHECK_EQUAL(dbManager.dirsInBufferCount(), 0);
	BOOST_CHECK_EQUAL(dbManager.currentlyProcessedCount(), 0);
}

BOOST_AUTO_TEST_CASE(ThreeSubdirsAndNoFilesInTreeRootDir)
{
	FileDataDBManager dbManager("../../../InstrumentRecognizerTester/test_files/tree_root", ".txt");

	BOOST_CHECK_EQUAL(dbManager.filesInBufferCount(), 0);
	BOOST_CHECK_EQUAL(dbManager.dirsInBufferCount(), 3);
}

BOOST_AUTO_TEST_CASE(OneFileToGetThenNewDirectoryThenEmptyException)
{
	FileDataDBManager dbManager("../../../InstrumentRecognizerTester/test_files/single_file_and_single_directory", ".txt");

	BOOST_CHECK_EQUAL(dbManager.dirsInBufferCount(), 1);

	BOOST_CHECK_EQUAL(dbManager.filesInBufferCount(), 1);
	
	DataRecord firstRecord = dbManager.take();
	BOOST_CHECK_EQUAL(readEntireRecord(firstRecord), "one_and_only");

	BOOST_CHECK_EQUAL(dbManager.filesInBufferCount(), 0);
	
	DataRecord secondRecord = dbManager.take();
	BOOST_CHECK_EQUAL(readEntireRecord(secondRecord), "second hidden");

	BOOST_CHECK_EQUAL(dbManager.filesInBufferCount(), 0);
	BOOST_CHECK_EQUAL(dbManager.dirsInBufferCount(), 0);
	
	BOOST_CHECK_EQUAL(dbManager.currentlyProcessedCount(), 2);
	BOOST_CHECK_THROW(dbManager.take(), DataDBManager::NoMoreRecordsException);

	dbManager.release(firstRecord.dbKey);
	dbManager.release(secondRecord.dbKey);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()