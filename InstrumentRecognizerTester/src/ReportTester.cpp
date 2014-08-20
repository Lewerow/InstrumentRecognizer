#include <boost/test/auto_unit_test.hpp>
#include <mutex>

#include <ReportBuilder.h>

#include "mocks/ReportDBManagerMock.hpp"

struct TextBuilderFixture
{
	TextBuilderFixture() : 
		reportDBManagerMock(new ReportDBManagerMock), report(new TextReport(10)), builder(reportDBManagerMock)
	{}

	std::shared_ptr<ReportDBManagerMock> reportDBManagerMock;
	std::shared_ptr<Report> report;
	TextReportBuilder builder;
};

BOOST_AUTO_TEST_SUITE(Reports)
BOOST_AUTO_TEST_SUITE(Builders)

BOOST_FIXTURE_TEST_SUITE(Text, TextBuilderFixture)

BOOST_AUTO_TEST_CASE(Text)
{
	MOCK_EXPECT(reportDBManagerMock->getNextReportID).once().returns(Report::ID(9));

	Report::ID id = builder.startReport(nullptr);
	MOCK_EXPECT(reportDBManagerMock->putReport).once();
	builder.endReport(nullptr);
	
	MOCK_EXPECT(reportDBManagerMock->getReport).once().returns(report);
	BOOST_CHECK_EQUAL(report, builder.report(id));
}

BOOST_AUTO_TEST_CASE(CannotEndNorReturnNotStartedReport)
{
	BOOST_CHECK_THROW(builder.endReport(nullptr), AssertException);
	BOOST_CHECK_THROW(builder.endReport(nullptr), AssertException);
}

BOOST_AUTO_TEST_CASE(CannotStartSameReportTwice)
{
	MOCK_EXPECT(reportDBManagerMock->getNextReportID).once().returns(Report::ID(9));
	builder.startReport(nullptr);
	BOOST_CHECK_THROW(builder.startReport(nullptr), AssertException);
}

BOOST_AUTO_TEST_CASE(CreatingBeforeEndingAsserts)
{
	MOCK_EXPECT(reportDBManagerMock->getNextReportID).once().returns(Report::ID(9));
	Report::ID id = builder.startReport(nullptr);
	
	BOOST_CHECK_THROW(builder.report(id), AssertException);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()