#pragma once

#include "ClassifierObservers.h"

class DescriptionDBManager;
class ReportBuilder;

class ClassifierTester : public ClassifierObserver
{
public:
	ClassifierTester(void);
	virtual ~ClassifierTester(void);
};

class CorrectClassifierTester : public ClassifierTester
{
public:

	CorrectClassifierTester(std::shared_ptr<DescriptionDBManager>);
	
	virtual void notifyFinishedTraining(Classifier*);
	virtual void notifyStartedTraining(Classifier*);
	virtual void notifyIteration(Classifier*, unsigned int);

private:
	std::shared_ptr<DescriptionDBManager> descriptionDBManager;
	
	CorrectClassifierTester(const CorrectClassifierTester&) : descriptionDBManager(descriptionDBManager) {};

	CorrectClassifierTester& operator=(const CorrectClassifierTester&) {return *this;};
};