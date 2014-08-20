#include <string>

class DescriptionDBManager;

namespace setup
{
	DescriptionDBManager* description_db_manager_for_describing(const std::string& path);
	DescriptionDBManager* description_db_manager_for_classifying(const std::string& path);
}