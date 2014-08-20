#pragma once

#include <memory>
#include <vector>

#include "Reporter.h"

class ReportBuilder;
class DescriptionDBManager;
class ClassifierDBManager;
class ClassifierFactory;
class ClassifierObserver;

class ClassifierOverlord : public Reporter
{
public:
	ClassifierOverlord(std::shared_ptr<ClassifierFactory>, std::shared_ptr<ClassifierDBManager>, std::shared_ptr<DescriptionDBManager>);
	virtual ~ClassifierOverlord(void);

	void setObservers(const std::vector<std::shared_ptr<ClassifierObserver> >&);
	void teachOne();

private:
	std::shared_ptr<ClassifierFactory> classifierFactory;
	std::shared_ptr<ClassifierDBManager> classifierDBManager;
	std::shared_ptr<DescriptionDBManager> descriptionDBManager;

	std::vector<std::shared_ptr<ClassifierObserver> > observers;
};

