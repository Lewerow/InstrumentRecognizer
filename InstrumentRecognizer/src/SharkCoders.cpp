#include "SharkCoders.h"	

#include <algorithm>
#include <boost/lexical_cast.hpp>

SimpleCoder::CodeType SimpleCoder::generateCode(const ClassDescriptionBase& descriptions)
{
	typedef std::pair<ClassName, ClassCodeType> CodeEntry;

	int i = 0;
	CodeType code;

	for (auto& cls : descriptions)
		code.insert(CodeEntry(cls.first, i++));

	return code;
}

SimpleCoder::ClassCodeType SimpleCoder::encode(const ClassName& name, const CodeType& base)
{
	return base.at(name);
}

ClassName SimpleCoder::decode(SimpleCoder::ClassCodeType classCode, const CodeType& base)
{
	CodeType::const_iterator it = std::find_if(base.begin(), base.end(), [classCode](const std::pair<ClassName, ClassCodeType>& p){ return p.second == classCode; });
	IR_ASSERT(it != base.end(), "Code received but not found: " + boost::lexical_cast<std::string>(classCode));
	return it->first;
}