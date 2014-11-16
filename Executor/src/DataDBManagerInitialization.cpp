#include "DataDBManagerInitialization.h"

#include <fstream>

#include <DataDBManager.h>

namespace setup
{
	DataDBManager* file_data_db_manager(const std::string& path, const std::string& extension)
	{
		return new FileDataDBManager(path, extension);
	}
	
	DataDBManager* line_data_db_manager(const std::string& path, int classNameColumn)
	{
		std::ifstream str(path);
        if (!str.is_open() || !str.good())
            throw std::runtime_error("Failed to initialize LineDataDBManager");

		return new LineDataDBManager(str, classNameColumn);
	}
}