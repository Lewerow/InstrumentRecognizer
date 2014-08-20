#include "GeneralClassifierDataObserver.h"
#include "Classifier.h"
#include "LinearClassifier.h"
#include "RandomClassifier.h"

#include "ReportBuilder.h"

#include <chrono>

void GeneralDataClassifierObserver::notifyFinishedTraining(Classifier* c)
{
	IR_ASSERT(reportBuilder, "Must have a report builder to insert general data to report");
	c->accept(this);

	reportBuilder.get()->addTitle(c, title);
	reportBuilder.get()->addHeader(c, header);
	reportBuilder.get()->addFooter(c, footer);
	reportBuilder.get()->addDate(c, date);
}

void GeneralDataClassifierObserver::notifyStartedTraining(Classifier*)
{
	date = std::chrono::system_clock::now();
}

void GeneralDataClassifierObserver::visit(Classifier* c)
{
	title = "Unknown";
	header = "Unknown classifier type";
	footer = "";
}

void GeneralDataClassifierObserver::visit(RandomClassifier*)
{
	title = "RandomClassifier";
	header = "For reference";
	footer = "";
}

void GeneralDataClassifierObserver::visit(LinearClassifier* c)
{
	title = "LinearClassifier";
	header = "Results for linear classifier";
	footer = "End of results for linear classifier";
}

void GeneralDataClassifierObserver::visit(LASSOClassifier* c)
{
	title = "LASSOClassifier";
	header = "Results for LASSO classifier";
	footer = "End of results for LASSO classifier";
}
void GeneralDataClassifierObserver::visit(NearestNeighborClassifier* c)
{
	title = "NearestNeighborClassifier";
	header = "Results. Unfortunately, currently there is no option to check strategy. sorry for that";
	footer = "The end. Hope you enjoyed it";
}