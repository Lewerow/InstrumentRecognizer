#include "DataDBManagerInitialization.h"

#include <DataDBManager.h>

namespace setup
{
	DataDBManager* data_db_manager(const std::string& path, const std::string& extension)
	{
		return new FileDataDBManager(path, extension);
	}
}