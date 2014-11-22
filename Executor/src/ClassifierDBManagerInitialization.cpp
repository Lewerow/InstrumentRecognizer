#include "ClassifierDBManagerInitialization.h"

#include <ClassifierDBManager.h>

namespace setup
{
	ClassifierDBManager* classifier_db_manager(const std::string& path, std::shared_ptr<ClassifierFactory> factory)
	{
		//		return new XmlFileClassifierDBManager(path, factory);
		return new DummyClassifierDBManager;
	}
}