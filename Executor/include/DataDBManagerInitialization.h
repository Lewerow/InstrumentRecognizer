#include <string>

class DataDBManager;

namespace setup
{
	DataDBManager* file_data_db_manager(const std::string& path, const std::string& extension);
	DataDBManager* line_data_db_manager(const std::string& path, int classNameColumn);
}