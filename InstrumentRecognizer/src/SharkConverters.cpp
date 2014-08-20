#include "SharkConverters.h"
#include <complex>
#include <algorithm>
#include <set>

namespace Converters
{
	template<>
	ObjectDescriptionToRealVector::OutType ObjectDescriptionToRealVector::convert<Descriptor>(const ObjectDescriptionToRealVector::InType& desc)
	{
		shark::RealVector vec(desc.size() * 2);

		int i = 0;
		for (auto d : desc)
		{
			vec[i++] = d.real();
			vec[i++] = d.imag();
		}

		return vec;
	}

	template <>
	ObjectDescriptionToRealVector::OutType ObjectDescriptionToRealVector::convert<double>(const ObjectDescriptionToRealVector::InType& desc)
	{
		shark::RealVector vec(desc.size());

		int i = 0;
		for (auto d : desc)
			vec[i++] = std::abs(d);
	
		return vec;
	}


	ObjectDescriptionToRealVector::OutType ObjectDescriptionToRealVector::convert(const ObjectDescriptionToRealVector::InType& desc)
	{
		return convert<Descriptor>(desc);
	}

	std::vector<ObjectDescriptionToRealVector::OutType> ObjectDescriptionToRealVector::convertAll(const std::vector<ObjectDescriptionToRealVector::InType>& v)
	{
		std::vector<ObjectDescriptionToRealVector::OutType> result;
		result.reserve(v.size());

		for (auto& i : v)
			result.push_back(convert<double>(i));

		return result;
	}


	UnsignedIntToClassifierResults::OutType UnsignedIntToClassifierResults::convert(const UnsignedIntToClassifierResults::InType& in)
	{
		OutType v;
		v.push_back(std::make_pair(in, 1.0));
		return v;
	}

	std::vector<UnsignedIntToClassifierResults::OutType> UnsignedIntToClassifierResults::convertAll(const std::vector<UnsignedIntToClassifierResults::InType>& in)
	{
		std::vector<OutType> res;
		res.reserve(in.size());

		for (auto& i : in)
			res.push_back(convert(i));

		return res;
	}

	std::vector<Dummy::OutType> Dummy::convertAll(const std::vector<Dummy::InType>& desc)
	{
		return desc;
	}

	std::vector<UnsignedIntToSingleElementRealVector::OutType> UnsignedIntToSingleElementRealVector::convertAll(const std::vector<UnsignedIntToSingleElementRealVector::InType>& input)
	{
		std::vector<OutType> results;
		results.reserve(input.size());

		for (auto& i : input)
		{
			OutType singleResult(1);
			singleResult[0] = i;
			results.push_back(singleResult);
		}

		return results;
	}

	SingleElementRealVectorToClassifierResults::OutType SingleElementRealVectorToClassifierResults::convert(const SingleElementRealVectorToClassifierResults::InType& val)
	{
		OutType results;
		long smaller = std::floor(val[0]);
		double diff = val[0] - smaller;
		
		results.push_back(std::make_pair(smaller, 1 - diff));
		results.push_back(std::make_pair(smaller + 1, diff));

		return results;
	}

	std::vector<SingleElementRealVectorToClassifierResults::OutType> SingleElementRealVectorToClassifierResults::convertAll(const std::vector<SingleElementRealVectorToClassifierResults::InType>& in)
	{
		std::vector<OutType> res;
		res.reserve(in.size());

		for (auto& i : in)
			res.push_back(convert(i));

		return res;
	}
}