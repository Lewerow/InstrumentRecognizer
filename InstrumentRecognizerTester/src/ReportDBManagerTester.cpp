#include <boost/test/auto_unit_test.hpp>

#include <Report.h>
#include <ReportDBManager.h>
#include <memory>
#include <set>

struct ReportDBManagerFixture
{
	ReportDBManagerFixture()
	{}
	
	ReportDBManager manager;
};

BOOST_AUTO_TEST_SUITE(DBManagers)
BOOST_FIXTURE_TEST_SUITE(Reports, ReportDBManagerFixture)

BOOST_AUTO_TEST_CASE(ReportDBManagerStartsAndLaterReturnsTheSame)
{
	Report::ID reportID = manager.getNextReportID();
	BOOST_CHECK_THROW(manager.getReport(reportID), ReportDBManager::NoSuchElement);
}

BOOST_AUTO_TEST_CASE(ThousandOfNextIdsDiffer)
{
	std::set<Report::ID> ids;

	std::size_t totalAdded = 1000;

	for(std::size_t i = 0; i < totalAdded; i++)
		ids.insert(manager.getNextReportID());

	BOOST_CHECK_EQUAL(ids.size(), totalAdded);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
