#ifndef DISCRETIZER_H_fdsifj3498j5ugfjfkldsfjdfnvkdfjvnrevijviurt
#define DISCRETIZER_H_fdsifj3498j5ugfjfkldsfjdfnvkdfjvnrevijviurt

#include <common_defs.h>

class TrivialDiscretizer
{
public:
	TrivialDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum);
	Discretized discretize(Descriptor value);
		
private:
	std::size_t mParts;
	const ClassDescriptionBase& mBase;
	std::size_t mAttribute;

	std::vector<Descriptor> thresholds;
};

#endif