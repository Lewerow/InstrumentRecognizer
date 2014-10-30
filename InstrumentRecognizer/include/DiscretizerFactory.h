#ifndef DISCRETIZER_FACTORY_H_fewfj9jfdjnkdscowiejf9gjurjg439fi4309fefjreifiuerfjijoifdwejfoiwejfiwejofew
#define DISCRETIZER_FACTORY_H_fewfj9jfdjnkdscowiejf9gjurjg439fi4309fefjreifiuerfjijoifdwejfoiwejfiwejofew

#include <memory>
#include <map>
#include <string>
#include <functional>

#include <Discretizer.h>

class DiscretizerFactory
{
public:
	typedef std::function<std::unique_ptr<Discretizer>(std::size_t, const ClassDescriptionBase&, std::size_t)> Creator;

	std::unique_ptr<Discretizer> getDefaultDiscretizer(std::size_t maxParts, const ClassDescriptionBase&, std::size_t attributeNum);
	std::unique_ptr<Discretizer> getDiscretizer(const std::string& name, std::size_t maxParts, const ClassDescriptionBase&, std::size_t attributeNum);

	void registerCreator(const std::string& name, Creator f);
	void setDefault(const std::string&);

private:
	std::string defaultCreatorName;
	std::map<std::string, Creator> creators;
};

#endif