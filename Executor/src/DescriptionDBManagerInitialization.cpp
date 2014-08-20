#include "DescriptionDBManagerInitialization.h"

#include <DescriptionDBManager.h>

namespace setup
{
	DescriptionDBManager* description_db_manager_for_describing(const std::string& path)
	{
		return new FileDescriptionDBManager(path);
	}

	DescriptionDBManager* description_db_manager_for_classifying(const std::string& path)
	{
		DescriptionDBManager* desc = new FileDescriptionDBManager(path);
		desc->loadDescriptions(path);

		return desc;
	}
}