#pragma once

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <DataDBManager.h>

MOCK_BASE_CLASS(DataDBManagerMock, DataDBManager)
{
	MOCK_METHOD_EXT(areRecordsAvailable, 0, bool(), areRecordsAvailable);
	MOCK_METHOD_EXT(take, 0, DataRecord (), take);
	MOCK_METHOD_EXT(release, 1, void (std::string), release);
};