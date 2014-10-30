#include <DiscretizerFactory.h>

std::unique_ptr<Discretizer> DiscretizerFactory::getDefaultDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum)
{
	return getDiscretizer(defaultCreatorName, maxParts, base, attributeNum);
}

std::unique_ptr<Discretizer> DiscretizerFactory::getDiscretizer(const std::string& name, std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum)
{
	IR_ASSERT(creators.count(name) == 0, "Default creator must exist");
	auto ptr = creators.at(name)(maxParts, base, attributeNum);
	ptr->teach();
	return ptr;
}

void DiscretizerFactory::registerCreator(const std::string& name, Creator f)
{
	IR_ASSERT(creators.count(name) == 0, "Cannot re-register same discretizer creator");
	creators.insert(std::make_pair(name, f));
}

void DiscretizerFactory::setDefault(const std::string& name)
{
	IR_ASSERT(creators.count(name) == 0, "Default creator must exist");
	defaultCreatorName = name;
}