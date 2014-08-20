#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <ClassifierFactory.h>

MOCK_BASE_CLASS(ClassifierFactoryMock, ClassifierFactory)
{
	MOCK_METHOD_EXT(registerBuilder, 2, void(ClassifierFactory::RegistrationKey, std::shared_ptr<Classifier::Builder>), registerBuilder);
	MOCK_METHOD_EXT(deregisterBuilder, 1, void(ClassifierFactory::RegistrationKey), deregisterBuilder);
	MOCK_METHOD_EXT(createClassifier, 2, std::shared_ptr<Classifier>(ClassifierFactory::RegistrationKey, const XMLNode&), createClassifierXMLNode);
	MOCK_METHOD_EXT(createClassifier, 2, std::shared_ptr<Classifier>(ClassifierFactory::RegistrationKey, const std::string&), createClassifierString);
	MOCK_METHOD_EXT(createDefaultClassifier, 1, std::shared_ptr<Classifier>(const XMLNode&), createDefaultClassifierXMLNode);
	MOCK_METHOD_EXT(createDefaultClassifier, 1, std::shared_ptr<Classifier>(const std::string&), createDefaultClassifierString);
	MOCK_METHOD_EXT(createDefaultClassifier, 0, std::shared_ptr<Classifier>(), createDefaultClassifierEmpty);

	MOCK_METHOD_EXT(createClassifier, 1, std::shared_ptr<Classifier>(ClassifierFactory::RegistrationKey), createNewClassifier);
};