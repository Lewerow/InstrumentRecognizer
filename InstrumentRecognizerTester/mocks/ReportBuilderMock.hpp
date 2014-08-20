#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <ReportBuilder.h>
#include <ReportDBManager.h>

MOCK_BASE_CLASS(ReportBuilderMock, ReportBuilder)
{
	ReportBuilderMock() : ReportBuilder(std::shared_ptr<ReportDBManager>())
	{}

	MOCK_METHOD_EXT(startReport, 1, Report::ID(Classifier*), startReport);
	MOCK_METHOD_EXT(endReport, 1, void(Classifier*), endReportClassifier);
	MOCK_METHOD_EXT(endReport, 1, void(Report::ID), endReportID);
	MOCK_METHOD_EXT(report, 1, std::shared_ptr<Report>(Report::ID), report);
	MOCK_METHOD_EXT(addTitle, 2, void(Classifier*, const std::string&), addTitle);
	MOCK_METHOD_EXT(addHeader, 2, void(Classifier*, const std::string&), addHeader);
	MOCK_METHOD_EXT(addFooter, 2, void(Classifier*, const std::string&), addFooter);
	MOCK_METHOD_EXT(addDate, 2, void(Classifier*, std::chrono::time_point<std::chrono::system_clock>), addDate);
	MOCK_METHOD_EXT(addTrainingDuration, 2, (void(Classifier*, std::chrono::system_clock::duration)), addTrainingDuration);
	MOCK_METHOD_EXT(addNumberOfIterations, 2, (void(Classifier*, unsigned int)), addNumberOfIterations);
	MOCK_METHOD_EXT(addClassifiedCorrectSingle, 3, void(Classifier*, const ClassName&, unsigned int), addClassifiedGoodSingle);
	MOCK_METHOD_EXT(addClassifiedIncorrectSingle, 3, void(Classifier*, const ClassName&, unsigned int), addClassifiedBadSingle);
	MOCK_METHOD_EXT(addClassifiedIncorrectTotal, 2, void(Classifier*, unsigned int), addClassifiedBadTotal);
	MOCK_METHOD_EXT(addClassifiedCorrectTotal, 2, void(Classifier*, unsigned int), addClassifiedGoodTotal);
	MOCK_METHOD_EXT(addRMSSingle, 3, void(Classifier*, const ClassName&, double), addRMSSingle);
	MOCK_METHOD_EXT(addRMSTotal, 2, void(Classifier*, double), addRMSTotal);
	MOCK_METHOD_EXT(addSpreadSingle, 3, void(Classifier*, const ClassName&, double), addSpreadSingle);
	MOCK_METHOD_EXT(addSpreadTotal, 2, void(Classifier*, double), addSpreadTotal);
	MOCK_METHOD_EXT(addClassToNumberMapping, 3, void(Classifier*, const ClassName&, int), addClassToNumberMapping);
	MOCK_METHOD_EXT(addTestObjectCount, 3, void(Classifier*, const ClassName&, unsigned int), addTestObjectCount);
	MOCK_METHOD_EXT(addTrainingObjectCount, 3, void(Classifier*, const ClassName&, unsigned int), addTrainingObjectCount);
	MOCK_METHOD_EXT(addTotalTestObjectCount, 2, void(Classifier*, unsigned int), addTotalTestObjectCount);
	MOCK_METHOD_EXT(addTotalTrainingObjectCount, 2, void(Classifier*, unsigned int), addTotalTrainingObjectCount);
	MOCK_METHOD_EXT(addClassificationResult, 4, void(Classifier*, const ClassName&, const ObjectDescription&, const ClassName&), addClassificationResult);

	MOCK_METHOD_EXT(assemble, 1, std::shared_ptr<Report>(Report::ID), assemble);
};