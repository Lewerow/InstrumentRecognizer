#pragma once

#include "common_defs.h"

class Report
{
public:

	typedef unsigned int ID;
	
	Report(ID newId) : id(newId){};
	virtual ~Report(void);

	ID id;
	std::string title;

	virtual std::string print() = 0;
};

class TextReport : public Report
{
public:
	TextReport(ID newId) : Report(newId) {}
	
	virtual std::string print();

private:
	friend class TextReportBuilder;
	std::string text;
};

