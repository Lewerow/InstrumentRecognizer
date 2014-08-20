#pragma once

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <Classifier.h>

#include <common_defs.h>

MOCK_BASE_CLASS(ClassifierMock, Classifier)
{
	MOCK_METHOD_EXT(doSetInputData, 1, void(const ClassDescriptionBase&), doSetInputData);
	MOCK_METHOD_EXT(doMakeBuilder, 0, Classifier::Builder*(void), doMakeBuilder);
	MOCK_METHOD_EXT(doRun, 0, void(void), doRun);
	MOCK_METHOD_EXT(doStop, 0, void(void), doStop);
	MOCK_METHOD_EXT(doCalculation, 1, ClassifierResults(const ObjectDescription&), doCalculation);
	MOCK_METHOD_EXT(doClassification, 1, ClassName(const ClassifierResults&), doClassification);
};

MOCK_BASE_CLASS(ClassifierBuilderMock, Classifier::Builder)
{
	MOCK_METHOD_EXT(build, 0, Classifier*(), buildDefault);
	MOCK_METHOD_EXT(build, 1, Classifier*(const XMLNode&), buildXMLNode);
	MOCK_METHOD_EXT(build, 1, Classifier*(const std::string&), buildString);	
	MOCK_METHOD_EXT(dismantleToXML, 1, std::unique_ptr<XMLNode>(std::shared_ptr<Classifier>), dismantleToXML);
	MOCK_METHOD_EXT(dismantleToText, 1, std::string(std::shared_ptr<Classifier>), dismantleToText);
};
