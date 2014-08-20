#include <type_traits>

/**
* \def ADD_CODER_TRAIT(TYPE_NAME)
* \brief Sets the is_coder trait to true for given type
*
* Classes that have is_coder trait must satisfy following conditions:
* - have typedefed CodeType
* - have typedefed ClassCodeType
* - have static method CodeType generateCode(const ClassDescriptionBase&). Type specifiers (&, const, volatile, mutable ...) may vary
* - have static method ClassCodeType encode(ClassName, CodeType). Type specifiers (&, const, volatile, mutable ...) may vary
* - have static method ClassName decode(ClassCodeType, CodeType). Type specifiers (&, const, volatile, mutable ...) may vary
* - invariant: Coder::decode(Coder::encode(x, t), t) == x
* - invariant: Coder::encode(Coder::decode(c, t), t) == c
* \param TYPE_NAME is the name of type that is to be given is_coder trait
*/
#define ADD_CODER_TRAIT(TYPE_NAME) template<> struct is_coder<TYPE_NAME> : std::true_type \
	{\
	private: is_coder()\
		{\
			TYPE_NAME::CodeType code = TYPE_NAME::generateCode(ClassDescriptionBase());\
			TYPE_NAME::ClassCodeType c = TYPE_NAME::encode(ClassName(), code);\
			ClassName y = TYPE_NAME::decode(c, code);\
		}\
	};

template<class T> struct is_coder : std::false_type {};