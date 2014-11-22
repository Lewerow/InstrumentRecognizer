#include <ClassifierResults.h>

ClassifierResults singleResultClass(const ClassName& name)
{
	ClassifierResults res;
	res.emplace(name, 1);
	return res;
}