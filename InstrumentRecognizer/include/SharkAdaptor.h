#ifdef BUILD_WITH_SHARK
#pragma once

#include <shark/Data/Dataset.h>

#include "ClassifierResults.h"
#include "SharkConverters.h"
#include "SharkCoders.h"
#include <memory>
#include <type_traits>

/**
* \class SharkClassifierAdaptor
* \brief Policy based adaptor for creating interface between application and Shark library
* \tparam OutputConverterClass must have is_converter trait
* \tparam InputConverterClass must have is_converter trait, InputConverter::InType must be equal to ObjectDescription
* \tparam CoderClass must have is_coder trait
* \sa is_coder
* \sa is_converter
* 
* Currently suitable only for supervised training, because of LabeledData as SharkInputType, but may be later easily customized
*/

template<class KeyConverterClass = Converters::Dummy, 
	class OutputConverterClass = Converters::UnsignedIntToClassifierResults,
	class ValueConverterClass = Converters::ObjectDescriptionToRealVector, 
	class CoderClass = SimpleCoder>
class SharkAdaptor
{
public:

	typedef CoderClass Coder;
	typedef typename Coder::CodeType Code;
	typedef typename Coder::ClassCodeType ClassCode;

	typedef KeyConverterClass KeyConverter;
	typedef typename KeyConverter::InType AdaptorInputKey;
	typedef typename KeyConverter::OutType SharkInputKey;

	typedef OutputConverterClass OutputConverter;
	typedef typename OutputConverter::InType OutputConverterInput;
	typedef typename OutputConverter::OutType OutputConverterOutput;

	typedef std::pair<ClassCode, double> AdaptorMiddleResult;
	typedef std::vector<AdaptorMiddleResult> AdaptorMiddleOutput;

	typedef ValueConverterClass ValueConverter;
	typedef typename ValueConverter::InType AdaptorInputValue;
	typedef typename ValueConverter::OutType SharkInputValue;

	typedef shark::LabeledData<SharkInputValue, SharkInputKey> SharkInputType;
	typedef ClassifierResults AdaptorOutput;

	static_assert(is_converter<KeyConverter>::value, "Key converter (first template parameter of SharkAdaptor) must have converter trait");
	static_assert(is_converter<ValueConverter>::value, "Value converter (second template parameter of SharkAdaptor) must have converter trait");
	static_assert(is_single_converter<OutputConverter>::value, "Output converter (third template parameter of SharkAdaptor) must have single converter trait");
	static_assert(is_coder<Coder>::value, "Coder (fourth template parameter of SharkAdaptor) must have coder trait");
	static_assert(std::is_same<AdaptorInputValue, ObjectDescription>::value, "Input converter must have ObjectDescription as InType to be used in SharkAdaptor");
	static_assert(std::is_same<AdaptorMiddleOutput, OutputConverterOutput>::value, "Adaptor middle output must be a vector of pairs: Code, double (probability)");
	
	SharkAdaptor()
	{}

	void setSharkInput(const ClassDescriptionBase& base)
	{
		code = Coder::generateCode(base);
		std::pair<std::vector<AdaptorInputKey>, std::vector<AdaptorInputValue> > keysVals = splitUp(base);
		std::vector<SharkInputKey> inputKeys = KeyConverter::convertAll(keysVals.first);
		std::vector<SharkInputValue> inputValues = ValueConverter::convertAll(keysVals.second);

		sharkInput = merge(inputKeys, inputValues);
	};

	AdaptorOutput toResults(const OutputConverterInput& out) const
	{
		AdaptorMiddleOutput halfway = OutputConverter::convert(out);
		
		ClassifierResults res;
		for (auto& i : halfway)
			res.push(SingleClassifierResult(Coder::decode(i.first, code), i.second));

		return res;
	}

	SharkAdaptor(const SharkAdaptor&) = delete;
	SharkAdaptor(const SharkAdaptor&&) = delete;
	SharkAdaptor& operator=(const SharkAdaptor&) = delete;
	SharkAdaptor& operator=(const SharkAdaptor&&) = delete;

protected:
		
	Code code;
	SharkInputType sharkInput;

private:
	std::pair<std::vector<AdaptorInputKey>, std::vector<AdaptorInputValue> > splitUp(const ClassDescriptionBase& base)
	{
		std::size_t objectCount = countObjects(base);
		
		std::vector<AdaptorInputKey> inputKeys;
		inputKeys.reserve(objectCount);
		std::vector<AdaptorInputValue> inputValues;
		inputValues.reserve(objectCount);

		for (auto& cls : base)
		{
			for (auto& obj : cls.second)
			{
				inputKeys.push_back(Coder::encode(cls.first, code));
				inputValues.push_back(obj);
			}
		}

		return std::make_pair(inputKeys, inputValues);
	}

	std::size_t countObjects(const ClassDescriptionBase& base)
	{
		std::size_t result = 0;
		for (auto& cls : base)
			result += cls.second.size();

		return result;
	}

	SharkInputType merge(const std::vector<SharkInputKey>& keys, const std::vector<SharkInputValue>& values)
	{
		IR_ASSERT(!(keys.empty() || values.empty()), "Both keys and values vectors need to have some elements");
		IR_ASSERT(keys.size() == values.size(), "Size of keys vector must be the same as size of values vector in SharkAdaptor");

		return shark::createLabeledDataFromRange(values, keys);
	}
};
#endif