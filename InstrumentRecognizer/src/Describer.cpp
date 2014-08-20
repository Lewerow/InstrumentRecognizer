#include <functional>

#include "Describer.h"
#include "common_defs.h"

#include <numeric>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std::placeholders;

namespace
{
	bool hasNameAndAttribute(const char* nodeName, const char* attrName, const char* attrValue, pugi::xml_node node)
	{
		if(strcmp(node.name(), nodeName))
			return false;

		return !strcmp(node.attribute(attrName).as_string(), attrValue);
	}

	pugi::xml_node getNode(const pugi::xml_document& doc, const char* attrType)
	{
		pugi::xml_node node = doc.find_node(std::bind(hasNameAndAttribute, "AudioDescriptor", "xsi:type", attrType, _1));
		if (node.empty())
			throw Describer::RequiredDescriptorNotFound(std::string("Descriptor: AudioDescriptor with attribute xsi:type at value: ") + attrType + " not found");

		return node;
	}

	Descriptor getAttributeScalarValue(MPEG7Describer::MPEG7Document& doc, const char* attrType)
	{
		pugi::xml_node node = getNode(doc, attrType);		
		return Descriptor(node.child("Scalar").text().as_double());
	}

	MPEG7DetailedDescriber::StatisticalDescription calculateStatisticalDescription(const std::vector<double> source)
	{
		if (source.empty())
			throw Describer::RequiredDescriptorNotFound(std::string("One of ScalarSeries audio descriptors contains invalid data"));

		double average = std::accumulate(source.begin(), source.end(), 0.0) / static_cast<double>(source.size());
		double variation = pow(std::accumulate(source.begin(), source.end(), 0.0, [average](double acc, double d){return acc + std::pow(average - d, 2.0);}) / static_cast<double>(source.size()), 0.5);

		return std::make_pair(average, variation);
	}

	std::vector<double> rawDataToVector(std::string& textual)
	{
		boost::trim(textual);
		textual.shrink_to_fit();
		std::vector<std::string> numbersTextual;
		boost::split(numbersTextual, textual, [](char c){ return c == ' '; });

		std::vector<double> series;
		series.reserve(numbersTextual.size());
		for (auto& i : numbersTextual)
			series.push_back(boost::lexical_cast<double>(i));

		return series;
	}

	MPEG7DetailedDescriber::StatisticalDescription getStatisticalDescription(MPEG7Describer::MPEG7Document& doc, const char* attrType, const char* attrValue)
	{
		pugi::xml_node type = getNode(doc, attrType);
		pugi::xml_node value = (attrValue == nullptr) ? type.child("SeriesOfScalar") : type.child(attrValue).child("SeriesOfScalar");

		std::string textual = value.child("Raw").text().as_string();
		
		if (textual.empty())
			throw Describer::RequiredDescriptorNotFound(std::string("Descriptor: AudioDescriptor with attribute xsi:type at value: ") + attrType + "/" + attrValue + " empty");

		return calculateStatisticalDescription(rawDataToVector(textual));
	}
}

Describer::Describer(std::istream* dataSource) : dataStream(dataSource)
{}


void Describer::setInputStream(std::istream* input)
{
	dataStream = input;
}

MPEG7Describer::MPEG7Describer(std::istream* dataSource) : Describer(dataSource)
{
}

ObjectDescription MPEG7Describer::generateObjectDescription()
{
	IR_ASSERT(dataStream == nullptr || !dataStream->eof(), "Empty data record given to MPEG7Describer!");
	pugi::xml_document mpeg7Doc;

	if (!mpeg7Doc.load(*dataStream, pugi::parse_full))
		throw InputParsingError("MPEG7Describer: invalid XML file loaded");

	return getObjectDescription(mpeg7Doc);
}

ObjectDescription MPEG7Describer::getObjectDescription(MPEG7Document& doc)
{
	const unsigned int numberOfMPEG7Descriptors = 7;

	ObjectDescription description(numberOfMPEG7Descriptors);
	description[0] = getAttributeScalarValue(doc, "LogAttackTimeType");
	description[1] = getAttributeScalarValue(doc, "SpectralCentroidType");
	description[2] = getAttributeScalarValue(doc, "TemporalCentroidType");
	description[3] = getAttributeScalarValue(doc, "HarmonicSpectralCentroidType");
	description[4] = getAttributeScalarValue(doc, "HarmonicSpectralDeviationType");
	description[5] = getAttributeScalarValue(doc, "HarmonicSpectralSpreadType");
	description[6] = getAttributeScalarValue(doc, "HarmonicSpectralVariationType");

	return description;
}

Describer* MPEG7Describer::clone()
{
	return new MPEG7Describer(nullptr);
}

MPEG7Describer::~MPEG7Describer(void)
{
}

MPEG7DetailedDescriber::MPEG7DetailedDescriber(std::istream* dataSource) : MPEG7Describer(dataSource)
{
}


ObjectDescription MPEG7DetailedDescriber::getObjectDescription(MPEG7Document& doc)
{
	const unsigned int numberOfMPEG7Descriptors = 13;

	ObjectDescription description(numberOfMPEG7Descriptors);
	ObjectDescription simpleDescription = MPEG7Describer::getObjectDescription(doc);
	std::copy(simpleDescription.begin(), simpleDescription.end(), description.begin());

	StatisticalDescription fundamentalFrequency = getStatisticalDescription(doc, "AudioFundamentalFrequencyType", nullptr);
	description[7] = fundamentalFrequency.first;
	description[8] = fundamentalFrequency.second;
	StatisticalDescription harmonicity = getStatisticalDescription(doc, "AudioHarmonicityType", "HarmonicRatio");
	description[9] = harmonicity.first;
	description[10] = harmonicity.second;
	StatisticalDescription harmonicityLimit = getStatisticalDescription(doc, "AudioHarmonicityType", "UpperLimitOfHarmonicity");
	description[11] = harmonicityLimit.first;
	description[12] = harmonicityLimit.second;

	return description;
}

Describer* MPEG7DetailedDescriber::clone()
{
	return new MPEG7DetailedDescriber(nullptr);
}

MPEG7DetailedDescriber::~MPEG7DetailedDescriber(void)
{
}
