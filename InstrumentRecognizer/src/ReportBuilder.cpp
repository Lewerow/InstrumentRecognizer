#include "ReportBuilder.h"
#include "ReportDBManager.h"

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include <numeric>

struct ReportBuilder::ReportStructure
{
	ReportStructure()
	{
	}

	~ReportStructure()
	{}

	std::shared_ptr<TextReport> assemble(Report::ID id)
	{return std::shared_ptr<TextReport>(new TextReport(id));}

	struct
	{
		std::string title;
		std::string heading;
		std::string footer;
		boost::optional<unsigned int> iterations;
		boost::optional<std::chrono::time_point<std::chrono::system_clock> > timestamp;
		boost::optional<std::chrono::system_clock::duration> trainingDuration;
	} general;
	 
	struct Results
	{
		boost::optional<double> rms;
		boost::optional<double> spread;
		boost::optional<double> accurracy;
		boost::optional<double> recall;
		boost::optional<double> precision;
		boost::optional<double> fmeasure;
		boost::optional<unsigned int> correct;
		boost::optional<unsigned int> incorrect;
		boost::optional<unsigned int> trainingObjects;
		boost::optional<unsigned int> testObjects;
	} totals;
	
	struct ClassData
	{
		boost::optional<int> mapping;
		Results results;
		boost::optional<std::unordered_map<ClassName, std::vector<ObjectDescription> > > classifiedAs;
	};

	boost::optional<std::unordered_map<ClassName, ClassData> > details;
};

class TextReportBuilder::Printer
{
public:
	virtual void printHeader(std::ostream&, Report::ID, std::shared_ptr<ReportStructure>);
	virtual void printFooter(std::ostream&, std::shared_ptr<ReportStructure>);
	virtual void printOverallResults(std::ostream&, std::shared_ptr<ReportStructure>);
	virtual void printClassesHeader(std::ostream&);
	virtual void printSingleClassResults(std::ostream&, const ClassName&, const ReportBuilder::ReportStructure::ClassData&);
	virtual void printDetailedResults(std::ostream&, std::shared_ptr<ReportStructure>);

private:
	virtual std::string serializeObjectDescription(const ObjectDescription&);
};

template<typename T>
std::ostream& operator<<(std::ostream& str, const boost::optional<T>& opt)
{
	if(!opt)
		str << "N/A";
	else
		str << opt.get();

	return str;
}

ReportBuilder::ReportBuilder(std::shared_ptr<ReportDBManager> repDBManager) : reportDBManager(repDBManager)
{
}

ReportBuilder::~ReportBuilder()
{}

Report::ID ReportBuilder::startReport(Classifier* classifier)
{
	IR_ASSERT(pendingReports.find(classifier) == pendingReports.end(), "Trying to add same report twice");
	
	Report::ID reportID = reportDBManager->getNextReportID();
	std::shared_ptr<ReportStructure> report(new ReportStructure);
	
	pendingReports.insert(std::make_pair(classifier, reportID));
	reportsCache.insert(std::make_pair(reportID, report));	

	return reportID;
}

void ReportBuilder::endReport(Classifier* classifier)
{
	IR_ASSERT(pendingReports.count(classifier) == 1, "Trying to finish already finished or not even started report");

	endReport(pendingReports.at(classifier));
	pendingReports.erase(pendingReports.find(classifier));
}

void ReportBuilder::endReport(Report::ID id)
{
	reportDBManager->putReport(id, assemble(id));
	finishedReports.push_back(reportsCache.at(id));
	reportsCache.erase(id);
}

void ReportBuilder::enableClassDetails(std::shared_ptr<ReportBuilder::ReportStructure> rs, const ClassName& cn)
{	
	if(!rs->details)
		rs->details = boost::make_optional(std::unordered_map<ClassName, ReportStructure::ClassData>());
	
	if(rs->details.get().count(cn) == 0)
		rs->details.get().insert(std::make_pair(cn, ReportStructure::ClassData()));
}

void ReportBuilder::addTitle(Classifier* c , const std::string& s)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->general.title = s;
}

void ReportBuilder::addHeader(Classifier* c, const std::string& s)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->general.heading = s;
}

void ReportBuilder::addFooter(Classifier* c, const std::string& s)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->general.footer = s;
}

void ReportBuilder::addNumberOfIterations(Classifier* c, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->general.iterations = i;
}

void ReportBuilder::addDate(Classifier* c, std::chrono::time_point<std::chrono::system_clock> t)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->general.timestamp = t;
}

void ReportBuilder::addSpreadSingle(Classifier* c, const ClassName& cn, double res)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);
		
	rep->details.get().at(cn).results.spread = res;
}

void ReportBuilder::addSpreadTotal(Classifier* c, double res)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));
	rep->totals.spread = res;
}

void ReportBuilder::addAccurracySingle(Classifier* c, const ClassName& cn, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.accurracy = i;
}

void ReportBuilder::addAccurracyTotal(Classifier* c, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.accurracy = i;
}

void ReportBuilder::addRecallSingle(Classifier* c, const ClassName& cn, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.recall = i;
}

void ReportBuilder::addRecallTotal(Classifier* c, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.recall = i;
}

void ReportBuilder::addPrecisionSingle(Classifier* c, const ClassName& cn, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.precision = i;
}

void ReportBuilder::addPrecisionTotal(Classifier* c, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.precision = i;
}

void ReportBuilder::addFMeasureSingle(Classifier* c, const ClassName& cn, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.fmeasure = i;
}

void ReportBuilder::addFMeasureTotal(Classifier* c, double i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.fmeasure = i;
}

void ReportBuilder::addTrainingDuration(Classifier* c, std::chrono::system_clock::duration t)
{	
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->general.trainingDuration = t;
}

void ReportBuilder::addClassifiedCorrectSingle(Classifier* c, const ClassName& cn, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);
		
	rep->details.get().at(cn).results.correct = i;
}
void ReportBuilder::addClassifiedIncorrectSingle(Classifier* c, const ClassName& cn, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");
	
	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.incorrect = i;
}
void ReportBuilder::addClassifiedIncorrectTotal(Classifier* c, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.incorrect = i;
}
void ReportBuilder::addClassifiedCorrectTotal(Classifier* c, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.correct = i;
}
void ReportBuilder::addRMSSingle(Classifier* c, const ClassName& cn, double d)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");
	
	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.rms = d;
}
void ReportBuilder::addRMSTotal(Classifier* c, double d)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.rms = d;
}
void ReportBuilder::addClassToNumberMapping(Classifier* c, const ClassName& cn, int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).mapping = i;
}
void ReportBuilder::addTestObjectCount(Classifier* c, const ClassName& cn, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");
	
	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.testObjects = i;
}
void ReportBuilder::addTrainingObjectCount(Classifier* c, const ClassName& cn, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");
	
	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, cn);

	rep->details.get().at(cn).results.trainingObjects = i;
}
void ReportBuilder::addTotalTestObjectCount(Classifier* c, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.testObjects = i;
}
void ReportBuilder::addTotalTrainingObjectCount(Classifier* c, unsigned int i)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");

	reportsCache.at(pendingReports.at(c))->totals.trainingObjects = i;
}

void ReportBuilder::addClassificationResult(Classifier* c, const ClassName& expected, const ObjectDescription& description, const ClassName& received)
{
	IR_ASSERT(pendingReports.count(c) == 1, "No pending report for given classifier");
	
	std::shared_ptr<ReportStructure> rep = reportsCache.at(pendingReports.at(c));	
	enableClassDetails(rep, expected);
	
	ReportStructure::ClassData& classData = rep->details.get().at(expected);

	if(!classData.classifiedAs)
		classData.classifiedAs = boost::make_optional(std::unordered_map<ClassName, std::vector<ObjectDescription> >());

	std::unordered_map<ClassName, std::vector<ObjectDescription> >& classifiedAs = classData.classifiedAs.get();
	
	if(classifiedAs.count(received) == 0)
		classifiedAs.insert(std::make_pair(received, std::vector<ObjectDescription>()));

	classifiedAs.at(received).push_back(description);
}

namespace
{
	template <typename T>
	boost::optional<T> accumulate_optional(boost::optional<T> opt, boost::optional<T> val)
	{
		if (opt.is_initialized())
			return val.get() + (opt.get());

		return val;
	}

	template <typename T>
	void divide(boost::optional<T>& t, int v)
	{
		if (t.is_initialized())
    		t = t.get() / v;
	}
}

void ReportBuilder::summarize()
{
	std::shared_ptr<ReportStructure> report(new ReportStructure);
	report->general.title = "Summary";

	for (auto& partial : finishedReports)
	{
		report->general.trainingDuration = accumulate_optional(report->general.trainingDuration, partial->general.trainingDuration);
		report->totals.rms = accumulate_optional(report->totals.rms, partial->totals.rms);
		report->totals.spread = accumulate_optional(report->totals.spread, partial->totals.spread);
		report->totals.accurracy = accumulate_optional(report->totals.accurracy, partial->totals.accurracy);
		report->totals.recall = accumulate_optional(report->totals.recall, partial->totals.recall);
		report->totals.precision = accumulate_optional(report->totals.precision, partial->totals.precision);
		report->totals.fmeasure = accumulate_optional(report->totals.fmeasure, partial->totals.fmeasure);
		report->totals.correct = accumulate_optional(report->totals.correct, partial->totals.correct);
		report->totals.incorrect = accumulate_optional(report->totals.incorrect, partial->totals.incorrect);
		report->totals.trainingObjects = accumulate_optional(report->totals.trainingObjects, partial->totals.trainingObjects);
		report->totals.testObjects = accumulate_optional(report->totals.testObjects, partial->totals.testObjects);
	}
	divide(report->totals.rms, finishedReports.size());
	divide(report->totals.spread, finishedReports.size());
	divide(report->totals.accurracy, finishedReports.size());
	divide(report->totals.recall, finishedReports.size());
	divide(report->totals.precision, finishedReports.size());
	divide(report->totals.fmeasure, finishedReports.size());
	divide(report->totals.correct, finishedReports.size());
	divide(report->totals.incorrect, finishedReports.size());
	divide(report->totals.trainingObjects, finishedReports.size());
	divide(report->totals.testObjects, finishedReports.size());

	reportsCache.insert(std::make_pair(99999, report));
	reportDBManager->putReport(99999, assemble(99999));
}

TextReportBuilder::TextReportBuilder(std::shared_ptr<ReportDBManager> mgr) : ReportBuilder(mgr), printer(new Printer)
{}

TextReportBuilder::~TextReportBuilder()
{}

std::shared_ptr<Report> TextReportBuilder::assemble(Report::ID id)
{
	IR_ASSERT(reportsCache.count(id) == 1, "Trying to assemble finished report");

	std::shared_ptr<ReportStructure> rs = reportsCache.at(id);

	std::stringstream str;
	printer->printHeader(str, id, rs);
	printer->printOverallResults(str, rs);
	
	if(rs->details)
	{
		printer->printClassesHeader(str);
		for(auto& i: rs->details.get())
			printer->printSingleClassResults(str, i.first, i.second);
	}

	printer->printDetailedResults(str, rs);
	printer->printFooter(str, rs);

	std::shared_ptr<TextReport> report(new TextReport(id));
	report->title = rs->general.title;
	report->text = str.str();

	return report;
}

std::shared_ptr<Report> TextReportBuilder::report(Report::ID id) const
{
	IR_ASSERT(reportsCache.count(id) == 0, "Trying to return not finished report");

	return reportDBManager->getReport(id);
}

void TextReportBuilder::Printer::printHeader(std::ostream& str, Report::ID id, std::shared_ptr<ReportStructure> rs)
{
	str << "Report ID: " << id << std::endl;
	str << rs->general.title << std::endl;
	str << rs->general.heading << std::endl;

	str << "General data" << std::endl;
	
	str << "Created on: ";
	if(rs->general.timestamp)
	{
		std::time_t time = std::chrono::system_clock::to_time_t(rs->general.timestamp.get());
		str << std::put_time(std::localtime(&time), "%Y-%m-%d, %H:%M:%S") << std::endl;
	}
	else
		str << "N/A" << std::endl;

	str << "Training time: " ;
	if(rs->general.trainingDuration)
		str << std::chrono::duration_cast<std::chrono::seconds>(rs->general.trainingDuration.get()).count() << " seconds" << std::endl;
	else
		str << "N/A" << std::endl;
	
	str << "Training iterations: " << rs->general.iterations;

	str << std::endl;
}

void TextReportBuilder::Printer::printFooter(std::ostream& str, std::shared_ptr<ReportStructure> rs)
{
	str << rs->general.footer << std::endl;
}

void TextReportBuilder::Printer::printOverallResults(std::ostream& str, std::shared_ptr<ReportStructure> rs)
{
	str << "Overall results" << std::endl;
	str << "Total training objects: " << rs->totals.trainingObjects << std::endl;
	str << "Total test objects: " << rs->totals.testObjects << std::endl;
	str << "Total correctly identified: " << rs->totals.correct << std::endl;
	str << "Total incorrectly identified: " << rs->totals.incorrect << std::endl;
	str << "Mean accurracy: " << rs->totals.accurracy << std::endl;
	str << "Mean recall: " << rs->totals.recall << std::endl;
	str << "Mean precision: " << rs->totals.precision << std::endl;
	str << "Mean F-measure: " << rs->totals.fmeasure << std::endl;
	str << "RMS: " << std::setprecision(4) << std::fixed << rs->totals.rms << std::endl;
	str << "Spread: " << std::setprecision(1) << std::fixed << rs->totals.spread << std::endl;
	str << std::endl;
}

void TextReportBuilder::Printer::printSingleClassResults(std::ostream& str, const ClassName& name, const ReportBuilder::ReportStructure::ClassData& data)
{
	str << "Class " << name << std::endl;
	str << "Mapped to number: " << data.mapping << std::endl;
	str << "Training objects: " << data.results.trainingObjects << std::endl;
	str << "Test objects: " << data.results.testObjects << std::endl;
	str << "Correctly identified: " << data.results.correct << std::endl;
	str << "Incorrectly identified: " << data.results.incorrect << std::endl;
	str << "Accurracy: " << std::setprecision(4) << std::fixed << data.results.accurracy << std::endl;
	str << "Recall: " << std::setprecision(4) << std::fixed << data.results.recall << std::endl;
	str << "Precision: " << std::setprecision(4) << std::fixed << data.results.precision << std::endl;
	str << "F-measure: " << std::setprecision(4) << std::fixed << data.results.fmeasure << std::endl;
	str << "RMS: " << std::setprecision(4) << std::fixed << data.results.rms << std::endl;
	str << "Spread: " << std::setprecision(0) << std::fixed << data.results.spread << std::endl;
	
	if(data.classifiedAs)
		for(auto& j: data.classifiedAs.get())
			str << j.first << ": " << j.second.size() << std::endl;
	
	str << std::endl;
}

void TextReportBuilder::Printer::printClassesHeader(std::ostream& str)
{
	str << "Data divided into classes" << std::endl;
}

void TextReportBuilder::Printer::printDetailedResults(std::ostream& str, std::shared_ptr<ReportStructure> rs)
{
	if(rs->details)
	{
		str << "Detailed classification data: " << std::endl;
		for(auto& expected: rs->details.get())
		{
			if(expected.second.classifiedAs)
			{
				for(auto& received: expected.second.classifiedAs.get())
					for(auto& object: received.second)
						str << serializeObjectDescription(object) << "of class " << expected.first << " classified as " << received.first << std::endl;
			}
		}
		str << std::endl;
	}
}

std::string TextReportBuilder::Printer::serializeObjectDescription(const ObjectDescription& desc)
{
	std::stringstream str;
	for(auto& i: desc)
//		str << i.real() << " " << i.imag() << " ";
		str << i << " ";

	return str.str();
}