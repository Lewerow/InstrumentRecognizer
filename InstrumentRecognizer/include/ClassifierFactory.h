#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <exception>

#include "common_defs.h"
#include "Classifier.h"

class ClassifierFactory
{
public:
	typedef std::string RegistrationKey;

	ClassifierFactory(void);
	virtual ~ClassifierFactory(void);
	
	virtual void registerBuilder(RegistrationKey, std::shared_ptr<Classifier::Builder>);
	virtual void deregisterBuilder(RegistrationKey);

	virtual void setDefaultBuilder(RegistrationKey);
	
	virtual std::shared_ptr<Classifier>  createClassifier(RegistrationKey, const XMLNode&);
	virtual std::shared_ptr<Classifier>  createClassifier(RegistrationKey, const std::string&);
	virtual std::shared_ptr<Classifier>  createClassifier(RegistrationKey);
	
	virtual std::shared_ptr<Classifier>  createDefaultClassifier(const XMLNode&);
	virtual std::shared_ptr<Classifier>  createDefaultClassifier(const std::string&);
	virtual std::shared_ptr<Classifier>  createDefaultClassifier();
	
	virtual std::unordered_set<RegistrationKey> availableClassifiers();

	struct ClassifierNotRegistered : public std::runtime_error
	{
		ClassifierNotRegistered(std::string str) : std::runtime_error(str) {}
	};
private:
	RegistrationKey defaultClassifierKey;
	std::unordered_map<RegistrationKey, std::shared_ptr<Classifier::Builder> > builders;
};

