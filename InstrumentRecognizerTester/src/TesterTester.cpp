#include <boost/test/auto_unit_test.hpp>
#include <ClassifierTesters.h>

#include "mocks/ClassifierMock.hpp"
#include "mocks/DescriptionDBManagerMock.hpp"
#include "mocks/ReportDBManagerMock.hpp"
#include "mocks/ReportBuilderMock.hpp"

struct TesterFixture
{
	TesterFixture() : repBuilder(new ReportBuilderMock)
	{
		createTestData();
	}
	
	void createTestData()
	{
		ObjectDescription objDesc11;
		objDesc11.push_back(Descriptor(100));
		objDesc11.push_back(Descriptor(200));
		objDesc11.push_back(Descriptor(300));;
		ClassDescription clsDesc1;
		clsDesc1.push_back(objDesc11);

		ObjectDescription objDesc21;
		objDesc21.push_back(Descriptor(-100));
		objDesc21.push_back(Descriptor(-200));
		objDesc21.push_back(Descriptor(-300));
		ClassDescription clsDesc2;
		clsDesc2.push_back(objDesc21);

		testData.insert(std::make_pair(ClassName("one"), clsDesc1));
		testData.insert(std::make_pair(ClassName("two"), clsDesc2));
	}
	
	DescriptionDBManagerMock descDB;
	ClassifierMock classifier;
	ClassDescriptionBase testData;
	std::shared_ptr<ReportBuilderMock> repBuilder;
};

BOOST_FIXTURE_TEST_SUITE(Testers, TesterFixture)

BOOST_AUTO_TEST_SUITE_END()