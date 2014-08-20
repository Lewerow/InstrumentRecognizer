#pragma once

#include <turtle/mock.hpp>
#include <ClassifierObservers.h>

MOCK_BASE_CLASS(ClassifierObserverMock, ClassifierObserver)
{
	MOCK_METHOD_EXT(notifyReceivedNewData, 2, void(Classifier*, ClassDescriptionBase&), notifyReceivedNewData);
	MOCK_METHOD_EXT(notifyStartedTraining, 1, void(Classifier*), notifyStartedTraining);
	MOCK_METHOD_EXT(notifyFinishedTraining, 1, void(Classifier*), notifyFinishedTraining);
	MOCK_METHOD_EXT(notifyIteration, 2, void(Classifier*, unsigned int), notifyIteration);

	MOCK_METHOD_EXT(setReportBuilder, 1, void(std::shared_ptr<ReportBuilder>), setReportBuilder);
};