#include <turtle/mock.hpp>

#include <RandomServer.h>

MOCK_BASE_CLASS(RandomServerMock, RandomServer)
{
	MOCK_METHOD_EXT(generate_uint, 2, unsigned int(unsigned int, unsigned int), generate_uint_2);
	MOCK_METHOD_EXT(generate_uint, 1, unsigned int(unsigned int), generate_uint_1);
};
