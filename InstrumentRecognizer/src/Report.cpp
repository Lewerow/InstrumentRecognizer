#include "Report.h"


Report::~Report(void)
{
}

std::string TextReport::print()
{
	return text;
}