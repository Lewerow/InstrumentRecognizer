#pragma once

#include "Report.h"

#include <memory>
#include <unordered_map>
#include <boost/filesystem.hpp>

class ReportDBManager
{
public:
	ReportDBManager(void);
	virtual ~ReportDBManager(void);

	virtual Report::ID getNextReportID();
	virtual std::shared_ptr<Report> getReport(Report::ID);
	virtual void putReport(Report::ID, std::shared_ptr<Report>);

	struct NoSuchElement : public std::out_of_range
	{
		NoSuchElement(std::string& str) : std::out_of_range(str) {}
	};
	
private:
	Report::ID currentID;
	std::unordered_map<Report::ID, std::shared_ptr<Report> > reports;

};

class FileReportDBManager : public ReportDBManager
{
public:
	FileReportDBManager(const boost::filesystem::path&);
	~FileReportDBManager(void);

	virtual void putReport(Report::ID, std::shared_ptr<Report>);

private:
	boost::filesystem::path destination;

	FileReportDBManager(const FileReportDBManager&) = delete;
	FileReportDBManager(const FileReportDBManager&&) = delete;
	FileReportDBManager& operator=(const FileReportDBManager&) = delete;
	FileReportDBManager& operator=(const FileReportDBManager&&) = delete;
};