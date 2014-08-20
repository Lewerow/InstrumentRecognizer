#include "TimeServer.h"


TimeServer::TimeServer(void)
{
}


TimeServer::~TimeServer(void)
{
}

std::chrono::time_point<std::chrono::system_clock> TimeServer::getCurrentTime()
{
	return std::chrono::system_clock::now();
}