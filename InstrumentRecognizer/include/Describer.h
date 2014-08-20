#pragma once

#include <string>
#include <vector>
#include <complex>
#include <istream>

#include "common_defs.h"


class Describer
{
public:
	virtual ObjectDescription generateObjectDescription() = 0;

	Describer(std::istream* dataSource = nullptr);
	virtual ~Describer(){};

	virtual void setInputStream(std::istream*);
	virtual Describer* clone() = 0;

	struct InputParsingError : std::invalid_argument
	{
		InputParsingError(std::string msg) : std::invalid_argument(msg) {}
	};

	struct RequiredDescriptorNotFound : std::runtime_error
	{
		RequiredDescriptorNotFound(std::string msg) : std::runtime_error(msg) {}
	};

protected:
	std::istream* dataStream;

private:
	void doSetInputStream(std::istream*){};
};

class MPEG7Describer : public Describer
{
public:
	typedef XMLDocument MPEG7Document;

	MPEG7Describer(std::istream* dataSource = nullptr);
	virtual ~MPEG7Describer();

	virtual ObjectDescription generateObjectDescription();
	virtual Describer* clone();

protected:
	virtual ObjectDescription getObjectDescription(MPEG7Document& doc);
};

class MPEG7DetailedDescriber : public MPEG7Describer
{
public:
	typedef std::pair<Descriptor, Descriptor> StatisticalDescription;
	MPEG7DetailedDescriber(std::istream* dataSource = nullptr);
	virtual ~MPEG7DetailedDescriber();

	virtual Describer* clone();

protected:
	virtual ObjectDescription getObjectDescription(MPEG7Document& doc);
};

