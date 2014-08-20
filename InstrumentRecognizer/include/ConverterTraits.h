#include <type_traits>

/**
* \def ADD_CONVERTER_TRAIT(TYPE_NAME)
* \brief Sets the is_converter trait to true for given type
*
* Classes that have is_converter trait must satisfy following conditions:
* - have typedefed InType
* - have typedefed OutType
* - have static function convertAll with signature std::vector<OutType> (std::vector<InType>). Type specifiers (&, const, volatile ...) may vary.
* \param TYPE_NAME is the name of type that is to be given is_converter trait
*/
#define ADD_CONVERTER_TRAIT(TYPE_NAME) template<> struct is_converter<TYPE_NAME> : std::true_type \
	{\
	private: is_converter()\
		{std::vector<TYPE_NAME::OutType> k = TYPE_NAME::convertAll(std::vector<TYPE_NAME::InType>()); }\
	};

template<class T> struct is_converter : std::false_type {};

/**
* \def ADD_SINGLE_CONVERTER_TRAIT(TYPE_NAME)
* \brief Sets the is_single_converter trait to true for given type
*
* Classes that have is_single_converter trait must satisfy following conditions:
* - have is_converter trait
* - have static function convert with signature OutType (InType). Type specifiers (&, const, volatile ...) may vary.
* \param TYPE_NAME is the name of type that is to be given is_converter trait
*/
#define ADD_SINGLE_CONVERTER_TRAIT(TYPE_NAME) template<> struct is_single_converter<TYPE_NAME> : std::true_type \
	{\
	static_assert(is_converter<TYPE_NAME>::value, "Single converter must be a converter!"); \
	private: is_single_converter()\
		{TYPE_NAME::OutType k = TYPE_NAME::convert(TYPE_NAME::InType()); }\
	};

template<class T> struct is_single_converter : std::false_type {};

