#include "ClassifierFactory.h"

#include "Classifier.h"
#include "common_defs.h"

#include <iterator>

ClassifierFactory::ClassifierFactory(void)
{
}


ClassifierFactory::~ClassifierFactory(void)
{
}

void ClassifierFactory::registerBuilder(RegistrationKey key, std::shared_ptr<Classifier::Builder> builder)
{
	IR_ASSERT(builders.count(key) == 0, "Trying to register builder that is already registered!");
	builders.insert(std::make_pair(key, builder));
}

void ClassifierFactory::deregisterBuilder(RegistrationKey key)
{
	IR_ASSERT(builders.count(key) == 1, "Trying to deregister builder that is not registered!");
	if(defaultClassifierKey == key)
		defaultClassifierKey.clear();

	builders.erase(key);
}	

void ClassifierFactory::setDefaultBuilder(RegistrationKey key)
{
	if(builders.count(key) == 0)
		throw ClassifierNotRegistered(key + " is not registered");

	defaultClassifierKey = key;
}

std::shared_ptr<Classifier>  ClassifierFactory::createClassifier(RegistrationKey key)
{
	if(builders.count(key) == 0)
		throw ClassifierNotRegistered(key + " is not registered");

	return std::shared_ptr<Classifier>(builders.at(key)->build());
}

std::shared_ptr<Classifier> ClassifierFactory::createClassifier(RegistrationKey key, const XMLNode& source)
{
	if(builders.count(key) == 0)
		throw ClassifierNotRegistered(key + " is not registered");
	
	return std::shared_ptr<Classifier>(builders.at(key)->build(source));
}

std::shared_ptr<Classifier> ClassifierFactory::createClassifier(RegistrationKey key, const std::string& source)
{
	if(builders.count(key) == 0)
		throw ClassifierNotRegistered(key + " is not registered");
	
	return std::shared_ptr<Classifier>(builders.at(key)->build(source));
}

std::shared_ptr<Classifier> ClassifierFactory::createDefaultClassifier(const XMLNode& source)
{
	if(defaultClassifierKey.empty())
		throw ClassifierNotRegistered("no default classifier set");

	return std::shared_ptr<Classifier>(builders.at(defaultClassifierKey)->build(source));
}

std::shared_ptr<Classifier> ClassifierFactory::createDefaultClassifier(const std::string& source)
{
	if(defaultClassifierKey.empty())
		throw ClassifierNotRegistered("no default classifier set");

	return std::shared_ptr<Classifier>(builders.at(defaultClassifierKey)->build(source));
}

std::shared_ptr<Classifier>  ClassifierFactory::createDefaultClassifier()
{
	if(defaultClassifierKey.empty())
		throw ClassifierNotRegistered("no default classifier set");
	
	return std::shared_ptr<Classifier>(builders.at(defaultClassifierKey)->build());
}

std::unordered_set<ClassifierFactory::RegistrationKey> ClassifierFactory::availableClassifiers()
{
	std::unordered_set<ClassifierFactory::RegistrationKey> indexes;
	for(auto i: builders)
		indexes.insert(i.first);

	return indexes;
}