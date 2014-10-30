#include <Discretizer.h>

Discretizer::Discretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum) : mParts(maxParts), mBase(base), mAttribute(attributeNum)
{
	teach();
}