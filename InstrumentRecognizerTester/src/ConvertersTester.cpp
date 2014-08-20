#include <boost/test/auto_unit_test.hpp>
#include <SharkConverters.h>

struct ToRealVectorFixture
{
	static_assert(std::is_same<ObjectDescription, Converters::ObjectDescriptionToRealVector::InType>::value, "ObjectDescriptionToRealVector must have ObjectDescription as InType");
	static_assert(std::is_same<shark::RealVector, Converters::ObjectDescriptionToRealVector::OutType>::value, "ObjectDescriptionToRealVector must have shark::RealVector as OutType");
	
	ToRealVectorFixture()
	{
		desc.push_back(2);
		desc.push_back(4);
		desc.push_back(7);
	}

	ObjectDescription desc;
};

BOOST_AUTO_TEST_SUITE(Converters)
BOOST_AUTO_TEST_SUITE(Input)
BOOST_FIXTURE_TEST_SUITE(ObjectDescription_To_RealVector, ToRealVectorFixture)
BOOST_AUTO_TEST_CASE(Default)
{
	shark::RealVector vec = Converters::ObjectDescriptionToRealVector::convert(desc);

	BOOST_REQUIRE_EQUAL(vec.size(), 6);
	BOOST_CHECK_CLOSE(vec[0], 2, 0.1);
	BOOST_CHECK_CLOSE(vec[1], 0, 0.1);
	BOOST_CHECK_CLOSE(vec[2], 4, 0.1);
	BOOST_CHECK_CLOSE(vec[3], 0, 0.1);
	BOOST_CHECK_CLOSE(vec[4], 7, 0.1);
	BOOST_CHECK_CLOSE(vec[5], 0, 0.1);
}

BOOST_AUTO_TEST_CASE(Descriptor_)
{
	shark::RealVector vec = Converters::ObjectDescriptionToRealVector::convert<Descriptor>(desc);

	BOOST_REQUIRE_EQUAL(vec.size(), 6);
	BOOST_CHECK_CLOSE(vec[0], 2, 0.01);
	BOOST_CHECK_CLOSE(vec[1], 0, 0.01);
	BOOST_CHECK_CLOSE(vec[2], 4, 0.01);
	BOOST_CHECK_CLOSE(vec[3], 0, 0.01);
	BOOST_CHECK_CLOSE(vec[4], 7, 0.01);
	BOOST_CHECK_CLOSE(vec[5], 0, 0.01);
}

BOOST_AUTO_TEST_CASE(Double)
{
	shark::RealVector vec = Converters::ObjectDescriptionToRealVector::convert<double>(desc);

	BOOST_REQUIRE_EQUAL(vec.size(), 3);
	BOOST_CHECK_CLOSE(vec[0], 2, 0.01);
	BOOST_CHECK_CLOSE(vec[1], 4, 0.01);
	BOOST_CHECK_CLOSE(vec[2], 7, 0.01);
}

BOOST_AUTO_TEST_CASE(OneByOneAndAllTogetherIsTheSame)
{
	ObjectDescription desc2;
	desc2.push_back(Descriptor(1));
	desc2.push_back(Descriptor(6));
	desc2.push_back(Descriptor(99));

	std::vector<ObjectDescription> v;
	v.push_back(desc);
	v.push_back(desc2);

	std::vector<shark::RealVector> all = Converters::ObjectDescriptionToRealVector::convertAll(v);
	shark::RealVector vec = Converters::ObjectDescriptionToRealVector::convert<double>(desc);
	shark::RealVector vec2 = Converters::ObjectDescriptionToRealVector::convert<double>(desc2);


	BOOST_REQUIRE_EQUAL(all.size(), 2);

	BOOST_REQUIRE_EQUAL(all[0].size(), vec.size());
	for (std::size_t i = 0; i < vec.size(); i++)
		BOOST_CHECK_CLOSE(all[0][i], vec[i], 0.01);
	
	BOOST_REQUIRE_EQUAL(all[1].size(), vec2.size());
	for (std::size_t i = 0; i < vec2.size(); i++)
		BOOST_CHECK_CLOSE(all[1][i], vec2[i], 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

struct OutputConverterFixture
{

	OutputConverterFixture() : expected(10)
	{
		expected[0] = 1;
		expected[1] = 2;
		expected[2] = 4;
		expected[3] = 6;
		expected[4] = 4;
		expected[5] = 5;
		expected[6] = 1;
		expected[7] = 5;
		expected[8] = 0;
		expected[9] = 4;
	}

	std::vector<unsigned int> expected;
};

BOOST_FIXTURE_TEST_SUITE(Output, OutputConverterFixture)
BOOST_AUTO_TEST_SUITE(Dummy)

BOOST_AUTO_TEST_CASE(Conversion)
{
	static_assert(std::is_same<Converters::Dummy::InType, unsigned int >::value, "InType of DummyConverter must be unsigned int");
	static_assert(std::is_same<Converters::Dummy::InType, Converters::Dummy::OutType>::value, "InType and OutType of DummyConverter must be the same");
	
	std::vector<Converters::Dummy::OutType> actual = Converters::Dummy::convertAll(expected);

	BOOST_REQUIRE_EQUAL(expected.size(), actual.size());
	BOOST_CHECK(std::equal(expected.begin(), expected.end(), actual.begin()));
}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()