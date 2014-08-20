#pragma once

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include "DescriberFactory.h"

MOCK_BASE_CLASS(DescriberFactoryMock, DescriberFactory)
{
	MOCK_METHOD_EXT(getDescriber, 0, Describer*(void), getDescriber);

};