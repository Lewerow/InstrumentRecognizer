#include <cstring>

#include <functional>
#include <boost/test/auto_unit_test.hpp>
#include <pugixml.hpp>

using namespace std::placeholders;


namespace
{
	bool hasNameAndAttribute(const char* nodeName, const char* attrName, const char* attrValue, pugi::xml_node node)
	{
		if(strcmp(node.name(), nodeName))
			return false;

		return !(node.find_attribute([=](pugi::xml_attribute attr) -> bool
		  {return !strcmp(attr.name(), attrName) && !strcmp(attr.value(), attrValue); })).empty();
	}
}
struct XmlFixture
{
	XmlFixture()
	{
		const char* content = "\
							  <root>\
							    <audio id = '1'>\
							      <desc id = 'abba'>\
								    <scalar>\
									  5.00\
									</scalar>\
							      </desc>\
							      <desc id = 'bba'>\
								    <scalar>\
									  15.00\
									</scalar>\
							      </desc>\
							      <desc id = 'aab'>\
								    <vector>\
									  15.00, 10, 22, 11.22\
									</vector>\
							      </desc>\
								</audio>\
							    <audio id = '2'>\
								  3\
								</audio>\
								<music id = '4'>\
  								  <scalar>\
								    2\
							      </scalar>\
								</music>\
						      </root>";

		BOOST_REQUIRE(doc.load(content));
	}

	pugi::xml_document doc;
	const char* content;
};


BOOST_FIXTURE_TEST_SUITE(XmlParser, XmlFixture)

BOOST_AUTO_TEST_CASE(NotEmptyDoc)
{
	BOOST_CHECK(!doc.empty());
}

BOOST_AUTO_TEST_CASE(FindingMusicNode)
{
	pugi::xml_node node = doc.find_node([](pugi::xml_node node){return !strcmp(node.name(), "music");});
	BOOST_CHECK_EQUAL(node.name(), std::string("music"));
	BOOST_CHECK_EQUAL(node.child("scalar").text().as_int(), 2);
}

BOOST_AUTO_TEST_CASE(FindingAudioNode2)
{
	pugi::xml_node node = doc.find_node(std::bind(hasNameAndAttribute, "audio", "id", "2", _1));

	BOOST_CHECK_EQUAL(node.name(), std::string("audio"));
	BOOST_CHECK_EQUAL(node.text().as_uint(), 3);
}


BOOST_AUTO_TEST_SUITE_END()