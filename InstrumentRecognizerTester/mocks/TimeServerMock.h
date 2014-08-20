#pragma once
#include <turtle/mock.hpp>

#include <TimeServer.h>

MOCK_BASE_CLASS(TimeServerMock, TimeServer)
{
	MOCK_METHOD_EXT(getCurrentTime, 0, std::chrono::time_point<std::chrono::system_clock>(void), getCurrentTime);
};