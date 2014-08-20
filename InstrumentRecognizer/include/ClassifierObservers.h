#pragma once

#include "Classifier.h"
#include "Reporter.h"

#include <string>
#include <chrono>

class ClassifierObserver : public Reporter
{
public:
	virtual void notifyReceivedNewData(Classifier*, ClassDescriptionBase&) {};
	virtual void notifyStartedTraining(Classifier*) {}
	virtual void notifyFinishedTraining(Classifier*) {}
	virtual void notifyIteration(Classifier*, unsigned int) {}

};

class ClassifierObservers : public ClassifierObserver, private std::list<std::shared_ptr<ClassifierObserver> >
{
public:
	virtual void add(std::shared_ptr<ClassifierObserver>);
	virtual void remove(std::shared_ptr<ClassifierObserver>);
	
	virtual void notifyReceivedNewData(Classifier*, ClassDescriptionBase&);
	virtual void notifyStartedTraining(Classifier*);
	virtual void notifyFinishedTraining(Classifier*);
	virtual void notifyIteration(Classifier*, unsigned int);
};

class RandomServer;

class DataCutterClassifierObserver : public ClassifierObserver
{
public:
	DataCutterClassifierObserver(const double cutDataBy, std::shared_ptr<RandomServer>);
	virtual ~DataCutterClassifierObserver(void);

	virtual void notifyReceivedNewData(Classifier*, ClassDescriptionBase&);

private:
	double cuttingFraction;
	std::shared_ptr<RandomServer> randomServer;
	
	void adjustSizes(std::size_t& totalSize, std::size_t& totalSizeModified, std::vector<std::size_t>& baseSizes, std::vector<std::size_t>& baseSizesModified);
	std::unordered_map<ClassName, std::size_t> getFinalSizes(const ClassDescriptionBase& base);
};

class TimeServer;

class BoundaryClassifierObserver : public ClassifierObserver
{
public:	
	BoundaryClassifierObserver(std::shared_ptr<TimeServer>);
	virtual ~BoundaryClassifierObserver();

	virtual void notifyStartedTraining(Classifier*);
	virtual void notifyFinishedTraining(Classifier*);
	virtual void notifyIteration(Classifier*, unsigned int);


	void setMaxDuration(std::chrono::system_clock::duration);
	void setMaxIterations(unsigned int);

private:

	class Impl;
	std::unique_ptr<Impl> pimpl;
};