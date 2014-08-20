#include "ClassifierTesterInitialization.h"

#include <ClassifierTesters.h>

namespace setup
{
	ClassifierTester* classifier_tester(std::shared_ptr<DescriptionDBManager> description_db_manager, std::shared_ptr<ReportBuilder> report_builder)
	{
		ClassifierTester* tester = new CorrectClassifierTester(description_db_manager);
		tester->setReportBuilder(report_builder);
		return tester;
	}
}