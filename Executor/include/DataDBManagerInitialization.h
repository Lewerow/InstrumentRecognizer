#include <string>

class DataDBManager;

namespace setup
{
	DataDBManager* data_db_manager(const std::string& path, const std::string& extension);
}