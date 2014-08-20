#pragma once

#include <memory>

class RandomServer
{
public:
	RandomServer();
	virtual ~RandomServer();
	virtual unsigned int generate_uint(unsigned int min, unsigned int max);
	virtual unsigned int generate_uint(unsigned int max);

	virtual unsigned int operator()(unsigned int max);
private:
	class Impl;
	std::unique_ptr<Impl> pimpl;
};