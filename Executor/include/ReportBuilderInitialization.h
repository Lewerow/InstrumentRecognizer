#include <vector>
#include <string>
#include <memory>

class ReportBuilder;
class ReportDBManager;

namespace setup
{
	ReportBuilder* report_builder(const std::string& report_format, std::shared_ptr<ReportDBManager> report_db_manager);
}