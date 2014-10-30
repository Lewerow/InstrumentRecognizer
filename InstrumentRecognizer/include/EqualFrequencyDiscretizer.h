#include <Discretizer.h>

class EqualFrequencyDiscretizer : public Discretizer
{
public:
	EqualFrequencyDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum);
	Discretized discretize(Descriptor value);
	void teach();

private:
	std::vector<Descriptor> thresholds;
};
