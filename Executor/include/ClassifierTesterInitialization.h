#include <memory>

class ClassifierTester;
class ReportBuilder;
class DescriptionDBManager;

namespace setup
{
	ClassifierTester* classifier_tester(std::shared_ptr<DescriptionDBManager> description_db_manager, std::shared_ptr<ReportBuilder> report_builder);
}