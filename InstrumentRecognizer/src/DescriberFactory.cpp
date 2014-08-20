#include "DescriberFactory.h"


DescriberFactory::DescriberFactory(Describer* describer) : describerPrototype(describer)
{
}

DescriberFactory::DescriberFactory(std::shared_ptr<Describer> describer) : describerPrototype(describer)
{
}

Describer* DescriberFactory::getDescriber()
{
	IR_ASSERT(describerPrototype.get(), "Prototype not set in describer factory!");
	return describerPrototype->clone();
}

DescriberFactory::~DescriberFactory(void)
{
}
