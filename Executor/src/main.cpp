#include <stdexcept>
#include <iostream>

#include "Configuration.h"

int main(int argc, char* argv[])
{

	std::unique_ptr<application::app> app(setup::app(argc, argv));
	app->run();

	return 0;
}
