#ifndef EQUAL_FREQUENCY_DISCRETIZER_H_foiwej9834jfgvrjenvfdkmncldfmkoi34ju4ingrjfkgnmfo35ifg3
#define EQUAL_FREQUENCY_DISCRETIZER_H_foiwej9834jfgvrjenvfdkmncldfmkoi34ju4ingrjfkgnmfo35ifg3

#include <memory>

#include <Discretizer.h>

class EqualFrequencyDiscretizer : public Discretizer
{
public:
	EqualFrequencyDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum);
	Discretized discretize(Descriptor value);
	void teach();
	
	static std::unique_ptr<EqualFrequencyDiscretizer> creator(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum);

private:
	std::vector<Descriptor> thresholds;
};

#endif