#include "RandomServer.h"

#include <random>
#include <chrono>

class RandomServer::Impl
{
public:
	Impl();
	unsigned int generate_uint(unsigned int min, unsigned int max);

private:
	std::mt19937_64 engine;
};

RandomServer::Impl::Impl()
{
	engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	engine.discard(100);
}

unsigned int RandomServer::Impl::generate_uint(unsigned int min, unsigned int max)
{
	std::uniform_int_distribution<> dist(min, max);
	return dist(engine);
}


RandomServer::RandomServer() : pimpl(new Impl)
{}

RandomServer::~RandomServer()
{}

unsigned int RandomServer::generate_uint(unsigned int min, unsigned int max)
{
	return pimpl->generate_uint(min, max);
}

unsigned int RandomServer::generate_uint(unsigned int max)
{
	return generate_uint(0, max);
}

unsigned int RandomServer::operator()(unsigned int max)
{
	return generate_uint(max - 1);
}