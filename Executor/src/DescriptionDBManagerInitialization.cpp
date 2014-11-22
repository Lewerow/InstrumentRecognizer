#include "DescriptionDBManagerInitialization.h"

#include <DescriptionDBManager.h>

namespace setup
{
	DescriptionDBManager* description_db_manager_for_describing(const std::string& path, int foldCount)
	{
		return new FileDescriptionDBManager(path, foldCount);
	}

	DescriptionDBManager* description_db_manager_for_classifying(const std::string& path, int foldCount)
	{
		DescriptionDBManager* desc = new FileDescriptionDBManager(path, foldCount);
		desc->loadDescriptions(path);

		return desc;
	}
}