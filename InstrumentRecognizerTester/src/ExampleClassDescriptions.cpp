#include "ExampleClassDescriptions.hpp"

ClassDescriptionBase getBasicClassDescriptionBase()
{
	ClassDescriptionBase base;
	ClassDescription a({ { 1, 2, 3 },{ 2, 3, 4 },{ 1.5, 2.5, 3.5 } });
	ClassDescription b({ { 10, 10, 10 },{ 12, 12, 12 },{ 11, 11, 11 } });
	ClassDescription c({ { 20, 22, 23 },{ 21, 21, 21 },{ 23.1, 23.4, 23.1 } });

	base.insert(std::make_pair("a", a));
	base.insert(std::make_pair("b", b));
	base.insert(std::make_pair("c", c));

	return base;
}