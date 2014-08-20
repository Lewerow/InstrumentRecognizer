#include "ReportDBManagerInitialization.h"

#include <ReportDBManager.h>

#include <boost/filesystem.hpp>

namespace setup
{
	ReportDBManager* report_db_manager(const std::string& path)
	{
		return new FileReportDBManager(boost::filesystem::path(path));
	}
}