#include "DescriberFactoryInitialization.h"

#include <Describer.h>
#include <DescriberFactory.h>

namespace setup
{
	DescriberFactory* describer_factory()
	{
		//		DescriberFactory* factory = new DescriberFactory(new MPEG7DetailedDescriber);
		DescriberFactory* factory = new DescriberFactory(new DummyDescriber);
		return factory;
	}
}