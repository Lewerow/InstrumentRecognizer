#pragma once

#include "common_defs.h"
#include "CoderTraits.h"

#include <unordered_map>

/**
* \class SimpleCoder
* \brief Simplest code generator possible - codes classes as consecutive integers starting from 0 (in order of iteration).
*
* Note that coding is consecutive, but platform and class count dependent (since ClassDescriptionBase may be implemented as unordered_map) 
*/
class SimpleCoder
{
public:
	typedef unsigned int ClassCodeType;
	typedef std::unordered_map<ClassName, ClassCodeType> CodeType;

	static CodeType generateCode(const ClassDescriptionBase&);
	static ClassName decode(ClassCodeType, const CodeType&);
	static ClassCodeType encode(const ClassName&, const CodeType&);
};

ADD_CODER_TRAIT(SimpleCoder);