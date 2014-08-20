#pragma once

#include <memory>
#include <boost/optional.hpp>

class ReportBuilder;

class Reporter
{
public:
	virtual void setReportBuilder(std::shared_ptr<ReportBuilder> repBuilder)
	{reportBuilder = boost::make_optional(repBuilder);}

	virtual ~Reporter()
	{reportBuilder.reset();}

protected: 
	boost::optional<std::shared_ptr<ReportBuilder> > reportBuilder;
};