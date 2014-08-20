#include <boost/test/auto_unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <string>

#include "mocks/ClassifierMock.hpp"
#include "mocks/DescriptionDBManagerMock.hpp"

#include <ClassifierObservers.h>
#include <LinearClassifier.h>
#include <NearestNeighborClassifier.h>
#include <LASSOClassifier.h>

namespace VLD
{
//	#include "E:\VLD\include\vld.h"
}

struct ClassifierFixture
{
	ClassifierFixture()
	{
		createTestData();
	}

	void createTestData()
	{
		ObjectDescription objDesc11;
		objDesc11.push_back(Descriptor(50));
		objDesc11.push_back(Descriptor(50));
		objDesc11.push_back(Descriptor(50));
		ObjectDescription objDesc12;
		objDesc12.push_back(Descriptor(10));
		objDesc12.push_back(Descriptor(10));
		objDesc12.push_back(Descriptor(10));
		ObjectDescription objDesc13;
		objDesc13.push_back(Descriptor(30));
		objDesc13.push_back(Descriptor(30));
		objDesc13.push_back(Descriptor(30));
		ObjectDescription objDesc14;
		objDesc14.push_back(Descriptor(40));
		objDesc14.push_back(Descriptor(40));
		objDesc14.push_back(Descriptor(40));
		ObjectDescription objDesc15;
		objDesc15.push_back(Descriptor(20));
		objDesc15.push_back(Descriptor(20));
		objDesc15.push_back(Descriptor(20));
		ClassDescription clsDesc1;
		clsDesc1.push_back(objDesc11);
		clsDesc1.push_back(objDesc12);
		clsDesc1.push_back(objDesc13);
		clsDesc1.push_back(objDesc14);
		clsDesc1.push_back(objDesc15);

		ObjectDescription objDesc21;
		objDesc21.push_back(Descriptor(100));
		objDesc21.push_back(Descriptor(100));
		objDesc21.push_back(Descriptor(100));
		ObjectDescription objDesc22;
		objDesc22.push_back(Descriptor(120));
		objDesc22.push_back(Descriptor(120));
		objDesc22.push_back(Descriptor(120));
		ObjectDescription objDesc23;
		objDesc23.push_back(Descriptor(140));
		objDesc23.push_back(Descriptor(140));
		objDesc23.push_back(Descriptor(140));
		ObjectDescription objDesc24;
		objDesc24.push_back(Descriptor(160));
		objDesc24.push_back(Descriptor(160));
		objDesc24.push_back(Descriptor(160));
		ObjectDescription objDesc25;
		objDesc25.push_back(Descriptor(150));
		objDesc25.push_back(Descriptor(150));
		objDesc25.push_back(Descriptor(150));
		ClassDescription clsDesc2;
		clsDesc2.push_back(objDesc21);
		clsDesc2.push_back(objDesc22);
		clsDesc2.push_back(objDesc23);
		clsDesc2.push_back(objDesc24);
		clsDesc2.push_back(objDesc25);
	
		testData.insert(std::make_pair(ClassName("one"), clsDesc1));
		testData.insert(std::make_pair(ClassName("two"), clsDesc2));
	}

	DescriptionDBManagerMock descriptionDBManager;
	ClassDescriptionBase testData;
};

BOOST_AUTO_TEST_SUITE(Classifiers)

typedef boost::mpl::list<LinearClassifier, NearestNeighborClassifier, LASSOClassifier> ClassifiersList;
BOOST_FIXTURE_TEST_CASE_TEMPLATE(BasicValidation, ClassifierType, ClassifiersList, ClassifierFixture)
{
	MOCK_EXPECT(descriptionDBManager.getDescriptions).once().returns(testData);
	ClassifierType classifier;
	classifier.setInputData(descriptionDBManager.getDescriptions());

	classifier.run();

	for (auto& i : testData)
	{
		for (auto& j : i.second)
		{
			BOOST_CHECK_EQUAL(classifier.classify(j), i.first);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()