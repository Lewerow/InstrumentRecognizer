#include <fstream>
#include <functional>

#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

#include <common_defs.h>
#include <Describer.h>

BOOST_AUTO_TEST_SUITE(Describers)

BOOST_AUTO_TEST_SUITE(Dummy)
BOOST_AUTO_TEST_CASE(SimplyExtractsDoublesFromStream)
{
    std::stringstream s;
    s << "1;2.3;6.66;5.44;";
    DummyDescriber desc;
    desc.setInputStream(&s);

    auto obj = desc.generateObjectDescription();
    BOOST_REQUIRE_EQUAL(4, obj.size());

    BOOST_CHECK_CLOSE(1, obj[0], 0.1);
    BOOST_CHECK_CLOSE(2.3, obj[1], 0.1);
    BOOST_CHECK_CLOSE(6.66, obj[2], 0.1);
    BOOST_CHECK_CLOSE(5.44, obj[3], 0.1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MPEG7)

BOOST_AUTO_TEST_CASE(MPEG7DescriberExistsAndRequiresStream)
{
	const double MAX_PERCENTAGE_DIFFERENCE = 0.01;
	std::ifstream is("../../../InstrumentRecognizerTester/test_files/mp7/bassoon/1953_bassoon_034_1_6_1.mp7");
	BOOST_REQUIRE(is.is_open());

	MPEG7Describer describer(&is);
	ObjectDescription description = describer.generateObjectDescription();

	ObjectDescription expected(7);
	expected[0] = -1.2067; // LogAttackTime
	expected[1] = 256.85; // SpectralCentroid
	expected[2] = 0.074510; // TemporalCentroid
	expected[3] = 619.47; // HarmonicSpectralCentroid
	expected[4] = 0.18898; // HarmonicSpectralDeviation
	expected[5] = 0.99624; // HarmonicSpectralSpread
	expected[6] = 0.15982; //HarmonicSpectralVariation

	BOOST_REQUIRE_EQUAL(description.size(), expected.size());

	for (std::size_t i = 0; i < expected.size(); i++)
	{
//		BOOST_CHECK_CLOSE(description.at(i).real(), expected.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
//		BOOST_CHECK_CLOSE(description.at(i).imag(), expected.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(description.at(i), expected.at(i), MAX_PERCENTAGE_DIFFERENCE);
	}
}

BOOST_AUTO_TEST_CASE(MPEG7DetailedDescriberExistsAndRequiresStream)
{
	const double MAX_PERCENTAGE_DIFFERENCE = 0.1;
	std::ifstream is("../../../InstrumentRecognizerTester/test_files/mp7/bassoon/1953_bassoon_034_1_6_1.mp7");
	BOOST_REQUIRE(is.is_open());

	MPEG7DetailedDescriber describer(&is);
	ObjectDescription description = describer.generateObjectDescription();

	ObjectDescription expected(13);
	expected[0] = -1.2067; // LogAttackTime
	expected[1] = 256.85; // SpectralCentroid
	expected[2] = 0.074510; // TemporalCentroid
	expected[3] = 619.47; // HarmonicSpectralCentroid
	expected[4] = 0.18898; // HarmonicSpectralDeviation
	expected[5] = 0.99624; // HarmonicSpectralSpread
	expected[6] = 0.15982; //HarmonicSpectralVariation
	expected[7] = 86.6904; //average FundamentalFrequency
	expected[8] = 54.7215; //FundamentalFrequency variation
	expected[9] = 0.55203; //average AudioHarmonicity
	expected[10] = 0.19118; //AudioHarmonicity variation
	expected[11] = 5851.97; //average UpperLimitOfHarmonicity
	expected[12] = 6967.366; //UpperLimitOfHarmonicity variation

	BOOST_REQUIRE_EQUAL(description.size(), expected.size());
	for (std::size_t i = 0; i < expected.size(); i++)
	{
//		BOOST_CHECK_CLOSE(description.at(i).real(), expected.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
//		BOOST_CHECK_CLOSE(description.at(i).imag(), expected.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(description.at(i), expected.at(i), MAX_PERCENTAGE_DIFFERENCE);
	}
}

BOOST_AUTO_TEST_CASE(ViolinHandledCorrectly)
{
	const double MAX_PERCENTAGE_DIFFERENCE = 0.01;
	std::ifstream is("../../../InstrumentRecognizerTester/test_files/mp7/viola/1002_viola_055_3_10_107.mp7");
	BOOST_REQUIRE(is.is_open());

	MPEG7Describer describer(&is);
	ObjectDescription description = describer.generateObjectDescription();
	
	ObjectDescription expected(7);
	expected[0] = -1.198057;
	expected[1] = 198.7499;
	expected[2] = 0.2377094;
	expected[3] = 1045.8944;
	expected[4] = 0.19090895;
	expected[5] = 0.6714315;
	expected[6] = 0.19529179;
    
	for(std::size_t i = 0; i < expected.size(); i++)
	{
//		BOOST_CHECK_CLOSE(description.at(i).real(), expected.at(i).real(), MAX_PERCENTAGE_DIFFERENCE);
//		BOOST_CHECK_CLOSE(description.at(i).imag(), expected.at(i).imag(), MAX_PERCENTAGE_DIFFERENCE);
		BOOST_CHECK_CLOSE(description.at(i), expected.at(i), MAX_PERCENTAGE_DIFFERENCE);
	}
}

BOOST_AUTO_TEST_CASE(MPEG7DescriberThrowsOnTXTInput)
{
	std::ifstream is("../../../InstrumentRecognizerTester/test_files/single_file/one.txt");
	BOOST_REQUIRE(is.is_open());
	
	MPEG7Describer describer(&is);
	BOOST_CHECK_THROW(describer.generateObjectDescription(), MPEG7Describer::InputParsingError);
}

BOOST_AUTO_TEST_CASE(MPEG7DescriberThrowsOnInputWithoutLogAttackTimeType)
{
	std::ifstream is("../../../InstrumentRecognizerTester/test_files/mp7_modified/bassoon/1953_bassoon_034_1_6_1_modified.mp7");
	BOOST_REQUIRE(is.is_open());
	
	MPEG7Describer describer(&is);
	BOOST_CHECK_THROW(describer.generateObjectDescription(), MPEG7Describer::RequiredDescriptorNotFound);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()