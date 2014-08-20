#pragma once

#include <memory>
#include <string>
#include <map>
#include <stdexcept>
#include <boost/filesystem.hpp>

class Classifier;
class ClassifierFactory;

class ClassifierDBManager
{
public:

	virtual void addClassifier(std::shared_ptr<Classifier>, const std::string&) = 0;
	virtual std::shared_ptr<Classifier> getClassifier(const std::string&) = 0;
	ClassifierDBManager(){}

	virtual ~ClassifierDBManager() {};

	struct CannotSaveClassifierException : std::runtime_error
	{
		CannotSaveClassifierException(std::string x) : std::runtime_error(x){}
	};

	struct CannotLoadClassifierException : std::runtime_error
	{
		CannotLoadClassifierException(std::string x) : std::runtime_error(x){}
	};

	struct NoSuchClassifier : std::out_of_range
	{
		NoSuchClassifier(std::string x) : std::out_of_range(x){}
	};
};

class XmlFileClassifierDBManager :	public ClassifierDBManager
{
public:
	XmlFileClassifierDBManager(const std::string& path, std::shared_ptr<ClassifierFactory> classifierFactory);
	virtual ~XmlFileClassifierDBManager();

	virtual void addClassifier(std::shared_ptr<Classifier>, const std::string&);
	virtual std::shared_ptr<Classifier> getClassifier(const std::string&);

private:
	boost::filesystem::path path;
	std::shared_ptr<ClassifierFactory> factory;
};