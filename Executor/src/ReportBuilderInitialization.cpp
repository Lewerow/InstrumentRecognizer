#include "ReportBuilderInitialization.h"

#include <ReportBuilder.h>

namespace setup
{
	ReportBuilder* report_builder(const std::string& report_format, std::shared_ptr<ReportDBManager> report_db_manager)
	{
		return new TextReportBuilder(report_db_manager);
	}
}