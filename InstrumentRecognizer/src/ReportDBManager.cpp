#include "ReportDBManager.h"

#include <fstream>
#include <boost/lexical_cast.hpp>

ReportDBManager::ReportDBManager(void) : currentID(0)
{
}

ReportDBManager::~ReportDBManager(void)
{
}

Report::ID ReportDBManager::getNextReportID()
{
	return currentID++;
}

std::shared_ptr<Report> ReportDBManager::getReport(Report::ID id)
{
	if(reports.count(id) == 0)
		throw NoSuchElement("Trying to access report with id: " + boost::lexical_cast<std::string>(id) + " which does not exist");

	return reports.at(id);
}

void ReportDBManager::putReport(Report::ID id, std::shared_ptr<Report> report)
{
	IR_ASSERT(reports.count(id) == 0, "Trying to insert second report with id: " + boost::lexical_cast<std::string>(id));

	reports.insert(std::make_pair(id, report));
}

FileReportDBManager::FileReportDBManager(const boost::filesystem::path& path) : destination(path)
{
	IR_ASSERT(boost::filesystem::is_directory(path), "FileReportDBManager expects to receive path to a directory. Received: " + path.string());
}

FileReportDBManager::~FileReportDBManager(void)
{}

void FileReportDBManager::putReport(Report::ID id, std::shared_ptr<Report> report)
{
	boost::filesystem::path filename = destination; 
	filename /= report->title + "_" + boost::lexical_cast<std::string>(id) + ".txt";
	
	int i = 0;
	while(boost::filesystem::exists(filename))
	{
		filename = destination;
		filename /= boost::filesystem::path( report->title + "_" + boost::lexical_cast<std::string>(id) + "_");
		filename += boost::filesystem::path(boost::lexical_cast<std::string>(i++));
		filename.replace_extension("txt");
	}

	std::ofstream str(filename.string());
	if(!str.is_open())
		throw std::runtime_error("Cannot access filesystem");

	str << report->print();
	str.close();

	ReportDBManager::putReport(id, report);
}