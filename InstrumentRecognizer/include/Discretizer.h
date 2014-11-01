#ifndef DISCRETIZER_FEfwef9034jf9renjnfdkvsdofjkurgikrekdowiodiwqodjwejferjfreiojferjfrefj
#define DISCRETIZER_FEfwef9034jf9renjnfdkvsdofjkurgikrekdowiodiwqodjwejferjfreiojferjfrefj

#include <common_defs.h>

class Discretizer
{
public:
	Discretizer(std::size_t maxParts, const ClassDescriptionBase&, std::size_t attributeNum);
    virtual ~Discretizer() {}

	virtual Discretized discretize(Descriptor value) = 0;
	virtual void teach() = 0;
	
protected:
	std::size_t mParts;
	const ClassDescriptionBase& mBase;
	std::size_t mAttribute;

};

#endif