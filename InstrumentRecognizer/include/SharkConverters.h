#ifdef BUILD_WITH_SHARK
#pragma once

#include "ClassifierResults.h"

#include "ConverterTraits.h"

#include <shark/Data/Dataset.h>

#include <unordered_map>
#include <vector>

namespace Converters
{
	/**
	* \class DummyConverter
	* \brief Returns the same vector it was given.
	*
	* This may be useful sometimes, e.g. for classifiers that require only one number on input
	*/
	class Dummy
	{
	public:
		typedef unsigned int InType;
		typedef unsigned int OutType;

		static std::vector<OutType> convertAll(const std::vector<InType>&);
	};

	class UnsignedIntToSingleElementRealVector
	{
	public:
		typedef unsigned int InType;
		typedef shark::RealVector OutType;

		static std::vector<OutType> convertAll(const std::vector<InType>&);
	};

	/**
	* \class ObjectDescriptionToRealVector
	* \brief Converts object description into vector of real values for Shark
	*
	* Note that complex values may be handled as two numbers (re, im) or as a single number, i.e. absolute value
	* Depends on the policy argument for convert function (default implementation uses std::complex)
	*/

	class ObjectDescriptionToRealVector
	{
	public:
		typedef ObjectDescription InType;
		typedef shark::RealVector OutType;

		template <typename T>
		static OutType convert(const InType&);

		static OutType convert(const InType&);
		static std::vector<OutType> convertAll(const std::vector<InType>&);
	};

	class UnsignedIntToClassifierResults
	{
	public:
		typedef unsigned int InType;
		typedef std::vector<std::pair<InType, double> > OutType;

		static OutType convert(const InType&);
		static std::vector<OutType> convertAll(const std::vector<InType>&);
	};

	class SingleElementRealVectorToClassifierResults
	{
	public:
		typedef shark::RealVector InType;
		typedef std::vector<std::pair<unsigned int, double> > OutType;

		static OutType convert(const InType&);
		static std::vector<OutType> convertAll(const std::vector<InType>&);
	};

}

ADD_CONVERTER_TRAIT(Converters::Dummy)
ADD_CONVERTER_TRAIT(Converters::UnsignedIntToSingleElementRealVector)
ADD_CONVERTER_TRAIT(Converters::ObjectDescriptionToRealVector)
ADD_SINGLE_CONVERTER_TRAIT(Converters::ObjectDescriptionToRealVector)
ADD_CONVERTER_TRAIT(Converters::UnsignedIntToClassifierResults)
ADD_SINGLE_CONVERTER_TRAIT(Converters::UnsignedIntToClassifierResults)
ADD_CONVERTER_TRAIT(Converters::SingleElementRealVectorToClassifierResults)
ADD_SINGLE_CONVERTER_TRAIT(Converters::SingleElementRealVectorToClassifierResults)

#endif