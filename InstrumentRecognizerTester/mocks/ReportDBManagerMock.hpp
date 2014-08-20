#pragma once

#include <turtle/mock.hpp>

#include <ReportDBManager.h>
#include <Report.h>

MOCK_BASE_CLASS(ReportDBManagerMock, ReportDBManager)
{
	MOCK_METHOD_EXT(getReport, 1, std::shared_ptr<Report>(Report::ID), getReport);
	MOCK_METHOD_EXT(getNextReportID, 0, Report::ID (void), getNextReportID);
	MOCK_METHOD_EXT(putReport, 2, void(Report::ID, std::shared_ptr<Report>), putReport);
};