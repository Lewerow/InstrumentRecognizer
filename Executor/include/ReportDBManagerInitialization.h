#include <string>

class ReportDBManager;

namespace setup
{
	ReportDBManager* report_db_manager(const std::string& path);
}