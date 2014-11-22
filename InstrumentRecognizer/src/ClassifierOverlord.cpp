#include <boost/lexical_cast.hpp>

#include "ClassifierOverlord.h"
#include "ClassifierObservers.h"

#include "ClassifierFactory.h"
#include "ClassifierDBManager.h"
#include "DescriptionDBManager.h"
#include "ReportBuilder.h"

ClassifierOverlord::ClassifierOverlord(std::shared_ptr<ClassifierFactory> factory, std::shared_ptr<ClassifierDBManager> classifierDBMgr, std::shared_ptr<DescriptionDBManager> descDBMgr) : 
	classifierFactory(factory), descriptionDBManager(descDBMgr), classifierDBManager(classifierDBMgr)
{
}

void ClassifierOverlord::setObservers(const std::vector<std::shared_ptr<ClassifierObserver> >& newObservers)
{
	observers.clear();
	observers.assign(newObservers.begin(), newObservers.end());
}

void ClassifierOverlord::teachOne()
{
	while (descriptionDBManager->areFoldsRemaining())
	{
		std::shared_ptr<Classifier> classifier = classifierFactory->createDefaultClassifier();
		IR_ASSERT(reportBuilder.get(), "Cannot teach classifier without report builder attached!");

		for(auto& o: observers)
		{
			o->setReportBuilder(reportBuilder.get());
			classifier->attachObserver(o);
		}

		Report::ID reportID = reportBuilder.get()->startReport(classifier.get());
		classifier->setInputData(descriptionDBManager->getTrainingDescriptions());
		classifier->run();
		classifier->stop();

		reportBuilder.get()->endReport(classifier.get());

		for(auto& o: observers)
			classifier->detachObserver(o);

		classifierDBManager->addClassifier(classifier, boost::lexical_cast<std::string>(reportID));
		descriptionDBManager->nextCrossValidationSet();
	}

	reportBuilder.get()->summarize();
}

ClassifierOverlord::~ClassifierOverlord(void)
{
}
