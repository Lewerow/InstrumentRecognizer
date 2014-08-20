#pragma once

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <DescriptionDBManager.h>

MOCK_BASE_CLASS(DescriptionDBManagerMock, DescriptionDBManager)
{
	MOCK_METHOD_EXT(startGathering, 0, void(), startGathering);
	MOCK_METHOD_EXT(finishGathering, 0, void(), finishGathering);
	MOCK_METHOD_EXT(reset, 0, void(), reset);

	MOCK_METHOD_EXT(addDescription, 2, void (const ClassName&, const ObjectDescription&), addDescription);
	MOCK_METHOD_EXT(loadDescriptions, 1, void (const std::string&), loadDescriptions);
	MOCK_METHOD_EXT(saveDescriptions, 1, void (const std::string&), saveDescriptions);
	MOCK_METHOD_EXT(getDescriptions, 0, const ClassDescriptionBase& (), getDescriptions);
};