#pragma once

#include <memory>

#include "Describer.h"


class DescriberFactory
{
public:
	DescriberFactory(Describer* describer = nullptr);
	DescriberFactory(std::shared_ptr<Describer> describer);
	virtual ~DescriberFactory(void);

	virtual Describer* getDescriber();

private:
	std::shared_ptr<Describer> describerPrototype;
};

