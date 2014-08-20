#include "ClassifierDBManager.h"
#include "ClassifierFactory.h"
#include "common_defs.h"
#include "Classifier.h"

#include <fstream>

XmlFileClassifierDBManager::XmlFileClassifierDBManager(const std::string& newPath, std::shared_ptr<ClassifierFactory> f) : path(newPath), factory(f)
{
	IR_ASSERT(boost::filesystem::exists(path), "Cannot use non-existent path");
	IR_ASSERT(factory.get() != nullptr, "Cannot use empty factory");
}

void XmlFileClassifierDBManager::addClassifier(std::shared_ptr<Classifier> c, const std::string& name)
{
	boost::filesystem::path p(path);
	p /= name;
	p.replace_extension("xml");
	std::ofstream str(p.string());

	if (!str.is_open())
		throw CannotSaveClassifierException("Cannot open output stream to file: " + p.string());

	std::unique_ptr<Classifier::Builder> builder = c->makeBuilder();
	std::unique_ptr<XMLNode> xml = builder->dismantleToXML(c);
	IR_ASSERT(xml.get() != nullptr, "Cannot save empty classifier");

	str << xml->text();
	str.close();
	if (str.fail())
		throw CannotSaveClassifierException("I/O error while writing classifier to: " + p.string());
}

std::shared_ptr<Classifier> XmlFileClassifierDBManager::getClassifier(const std::string& name)
{
	return std::shared_ptr <Classifier>(nullptr);
}

XmlFileClassifierDBManager::~XmlFileClassifierDBManager()
{
}
