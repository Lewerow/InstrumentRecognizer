#pragma once

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

#include <Describer.h>

MOCK_BASE_CLASS(DescriberMock, Describer)
{
	MOCK_METHOD_EXT(generateObjectDescription, 0, ObjectDescription(void), generateObjectDescription);
	MOCK_METHOD_EXT(clone, 0, Describer*(void), clone);

	MOCK_METHOD_EXT(setInputStream, 1, void(std::istream*), setInputStream);
};