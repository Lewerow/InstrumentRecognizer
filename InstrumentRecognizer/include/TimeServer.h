#pragma once

#include <chrono>

class TimeServer
{
public:
	TimeServer(void);
	virtual ~TimeServer(void);

	virtual std::chrono::time_point<std::chrono::system_clock> getCurrentTime();
};

