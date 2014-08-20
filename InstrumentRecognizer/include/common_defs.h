#pragma once

#include <exception>
#include <string>
#include <vector>
#include <unordered_map>
#include <complex>

#include <pugixml.hpp>

typedef std::string ClassName;
typedef std::complex<double> Descriptor;
typedef std::vector<Descriptor> ObjectDescription;
typedef std::vector<ObjectDescription> ClassDescription;
typedef std::unordered_map<ClassName, ClassDescription> ClassDescriptionBase;

typedef pugi::xml_document XMLDocument;
typedef pugi::xml_node XMLNode;

struct AssertException : public std::runtime_error
{
	AssertException(const std::string str) : std::runtime_error(str) {}
};

#define INSTRUMENT_RECOGNIZER_TESTS

#ifdef INSTRUMENT_RECOGNIZER_TESTS
#define IR_ASSERT(x, y) if(!(x)) throw AssertException(std::string(y) + " !!uncatchable AssertException (IR_ASSERT)!!")
#else
#define IR_ASSERT(x, y) assert(x)
#endif
