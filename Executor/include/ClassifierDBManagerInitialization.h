#include <string>
#include <memory>

class ClassifierDBManager;
class ClassifierFactory;

namespace setup
{
	ClassifierDBManager* classifier_db_manager(const std::string& path, std::shared_ptr<ClassifierFactory> factory);
}