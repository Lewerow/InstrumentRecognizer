#ifndef DISCRETIZER_H_fdsifj3498j5ugfjfkldsfjdfnvkdfjvnrevijviurt
#define DISCRETIZER_H_fdsifj3498j5ugfjfkldsfjdfnvkdfjvnrevijviurt

#include <Discretizer.h>

class EqualSizeDiscretizer : public Discretizer
{
public:
	EqualSizeDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum);
	Discretized discretize(Descriptor value);
		
private:
	void teach();
	std::vector<Descriptor> thresholds;
};

#endif