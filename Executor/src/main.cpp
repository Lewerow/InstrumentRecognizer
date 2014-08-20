#include <stdexcept>
#include <iostream>

#include "Configuration.h"

int main(int argc, char* argv[])
{

	try
	{
		std::unique_ptr<application::app> app(setup::app(argc, argv));
		app->run();
	}
	catch(std::exception ex)
	{
		std::cerr <<"Error occurred: " << ex.what() << " Terminating ..." << std::endl;
	}

	return 0;
}
