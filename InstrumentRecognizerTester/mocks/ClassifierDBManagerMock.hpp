#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <ClassifierDBManager.h>

MOCK_BASE_CLASS(ClassifierDBManagerMock, ClassifierDBManager)
{
	MOCK_METHOD_EXT(addClassifier, 2, void(std::shared_ptr<Classifier>, const std::string&), addClassifier);
	MOCK_METHOD_EXT(getClassifier, 1, std::shared_ptr<Classifier>(const std::string&), getClassifier);
};