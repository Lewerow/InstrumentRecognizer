#include <boost/test/auto_unit_test.hpp>

#include <SharkCoders.h>

struct CodersFixture
{
	CodersFixture()
	{
		base.insert(std::make_pair("Class1", ClassDescription()));
		base.insert(std::make_pair("Class2", ClassDescription()));
		base.insert(std::make_pair("Class3", ClassDescription()));
		base.insert(std::make_pair("Class4", ClassDescription()));
	}

	ClassDescriptionBase base;
};

BOOST_AUTO_TEST_SUITE(Coders)
BOOST_FIXTURE_TEST_SUITE(Simple, CodersFixture)

BOOST_AUTO_TEST_CASE(NoBaseNoCode)
{
	SimpleCoder::CodeType code = SimpleCoder::generateCode(ClassDescriptionBase());
	BOOST_REQUIRE(code.empty());
}

BOOST_AUTO_TEST_CASE(ClassesAreCodedWithConsecutiveNumbers)
{
	SimpleCoder::CodeType code = SimpleCoder::generateCode(base);

	BOOST_REQUIRE_EQUAL(code.size(), 4);
	BOOST_REQUIRE_EQUAL(code.count("Class1"), 1);
	BOOST_REQUIRE_EQUAL(code.count("Class2"), 1);
	BOOST_REQUIRE_EQUAL(code.count("Class3"), 1);
	BOOST_REQUIRE_EQUAL(code.count("Class4"), 1);

	BOOST_CHECK_NE(code.at("Class1"), code.at("Class2"));
	BOOST_CHECK_NE(code.at("Class1"), code.at("Class3"));
	BOOST_CHECK_NE(code.at("Class1"), code.at("Class4"));
	BOOST_CHECK_NE(code.at("Class2"), code.at("Class3"));
	BOOST_CHECK_NE(code.at("Class2"), code.at("Class4"));
	BOOST_CHECK_NE(code.at("Class3"), code.at("Class4"));

	for (auto& i : code)
	{
		BOOST_CHECK(base.count(i.first) > 0);
		BOOST_CHECK(i.second >= 0 && i.second < 4);
	}
}

BOOST_AUTO_TEST_CASE(EncodeDecode)
{
	SimpleCoder::CodeType code = SimpleCoder::generateCode(base);

	BOOST_CHECK_EQUAL(SimpleCoder::encode("Class1", code), code.at("Class1"));
	BOOST_CHECK_EQUAL(SimpleCoder::encode("Class2", code), code.at("Class2"));
	BOOST_CHECK_EQUAL(SimpleCoder::encode("Class3", code), code.at("Class3"));
	BOOST_CHECK_EQUAL(SimpleCoder::encode("Class4", code), code.at("Class4"));

	BOOST_CHECK_EQUAL(SimpleCoder::decode(SimpleCoder::encode("Class4", code), code), "Class4");
	BOOST_CHECK_EQUAL(SimpleCoder::decode(SimpleCoder::encode("Class3", code), code), "Class3");
	BOOST_CHECK_EQUAL(SimpleCoder::decode(SimpleCoder::encode("Class2", code), code), "Class2");
	BOOST_CHECK_EQUAL(SimpleCoder::decode(SimpleCoder::encode("Class1", code), code), "Class1");

	BOOST_CHECK_EQUAL(SimpleCoder::encode(SimpleCoder::decode(code.at("Class1"), code), code), code.at("Class1"));
	BOOST_CHECK_EQUAL(SimpleCoder::encode(SimpleCoder::decode(code.at("Class2"), code), code), code.at("Class2"));
	BOOST_CHECK_EQUAL(SimpleCoder::encode(SimpleCoder::decode(code.at("Class3"), code), code), code.at("Class3"));
	BOOST_CHECK_EQUAL(SimpleCoder::encode(SimpleCoder::decode(code.at("Class4"), code), code), code.at("Class4"));
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()