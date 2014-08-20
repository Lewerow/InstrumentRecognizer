#pragma once

#include <memory>
#include <chrono>
#include <unordered_map>
#include <iostream>

#include "Report.h"

class Classifier;
class ReportDBManager;

class ReportBuilder
{
public:
	ReportBuilder(std::shared_ptr<ReportDBManager>);
	virtual ~ReportBuilder();


	virtual Report::ID startReport(Classifier*);
	virtual void endReport(Classifier*);
	virtual void endReport(Report::ID);
	virtual std::shared_ptr<Report> report(Report::ID) const = 0;
	
	virtual void addTitle(Classifier*, const std::string&);
	virtual void addHeader(Classifier*, const std::string&);
	virtual void addFooter(Classifier*, const std::string&);
	virtual void addDate(Classifier*, std::chrono::time_point<std::chrono::system_clock>);
	virtual void addTrainingDuration(Classifier*, std::chrono::system_clock::duration);
	virtual void addNumberOfIterations(Classifier*, unsigned int);
	virtual void addClassifiedCorrectSingle(Classifier*, const ClassName&, unsigned int);
	virtual void addClassifiedIncorrectSingle(Classifier*, const ClassName&, unsigned int);
	virtual void addClassifiedCorrectTotal(Classifier*, unsigned int);
	virtual void addClassifiedIncorrectTotal(Classifier*, unsigned int);
	virtual void addSpreadSingle(Classifier*, const ClassName&, double);
	virtual void addSpreadTotal(Classifier*, double);
    virtual void addRMSSingle(Classifier*, const ClassName&, double);
	virtual void addRMSTotal(Classifier*, double);
	virtual void addClassToNumberMapping(Classifier*, const ClassName&, int);
	virtual void addTestObjectCount(Classifier*, const ClassName&, unsigned int);
	virtual void addTrainingObjectCount(Classifier*, const ClassName&, unsigned int);
	virtual void addTotalTestObjectCount(Classifier*, unsigned int);
	virtual void addTotalTrainingObjectCount(Classifier*, unsigned int);
	virtual void addClassificationResult(Classifier*, const ClassName& expected, const ObjectDescription& description, const ClassName& received);
	
protected:
	struct ReportStructure;

	virtual std::shared_ptr<Report> assemble(Report::ID) = 0;

	std::shared_ptr<ReportDBManager> reportDBManager;

	std::unordered_map<Classifier*, Report::ID> pendingReports;
	std::unordered_map<Report::ID, std::shared_ptr<ReportStructure> > reportsCache;

private:
	void enableClassDetails(std::shared_ptr<ReportBuilder::ReportStructure> rs, const ClassName& cn);

};

class TextReportBuilder : public ReportBuilder
{
public:
	TextReportBuilder(std::shared_ptr<ReportDBManager>);
	virtual ~TextReportBuilder();
	virtual std::shared_ptr<Report> report(Report::ID) const;

protected:
	virtual std::shared_ptr<Report> assemble(Report::ID);

private:
	class Printer;
	std::unique_ptr<Printer> printer;
};