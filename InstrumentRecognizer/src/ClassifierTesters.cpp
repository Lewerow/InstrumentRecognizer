#include "ClassifierTesters.h"
#include "DescriptionDBManager.h"
#include "Report.h"
#include "ReportBuilder.h"

#include <unordered_set>
#include <numeric>

ClassifierTester::ClassifierTester(void)
{
}


ClassifierTester::~ClassifierTester(void)
{
}

CorrectClassifierTester::CorrectClassifierTester(std::shared_ptr<DescriptionDBManager> descMgr) : 
	descriptionDBManager(descMgr)
{}

void CorrectClassifierTester::notifyFinishedTraining(Classifier* classifier)
{
	if(!reportBuilder)
		return;
	
	auto repBuilder = reportBuilder.get();

	const ClassDescriptionBase& desc = descriptionDBManager->getDescriptions();
	
	unsigned int classifiedCorrectlyTotal = 0;
	unsigned int classifiedIncorrectlyTotal = 0;
	std::vector<unsigned int> spreads;
	for(const auto& cls: desc)
	{
		std::unordered_set<ClassName> spreadCounter;

		unsigned int classifiedCorrectlySingle = 0;
		unsigned int classifiedIncorrectlySingle = 0;

		for (const auto& obj : cls.second)
		{
			ClassName result = classifier->classify(obj);
			repBuilder->addClassificationResult(classifier, cls.first, obj, result);

			if (result == cls.first)
				classifiedCorrectlySingle++;
			else
			{
				classifiedIncorrectlySingle++;
				if (!spreadCounter.count(result))
					spreadCounter.insert(result);
			}
		}
		
		repBuilder->addSpreadSingle(classifier, cls.first, spreadCounter.size());
		repBuilder->addClassifiedCorrectSingle(classifier, cls.first, classifiedCorrectlySingle);
		repBuilder->addClassifiedIncorrectSingle(classifier, cls.first, classifiedIncorrectlySingle);
		repBuilder->addTestObjectCount(classifier, cls.first, classifiedCorrectlySingle + classifiedIncorrectlySingle);
		
		classifiedCorrectlyTotal += classifiedCorrectlySingle;
		classifiedIncorrectlyTotal += classifiedIncorrectlySingle;
		spreads.push_back(spreadCounter.size());
	}

	IR_ASSERT(!spreads.empty(), "How in hell we got here, if there are no training objects??");

	repBuilder->addSpreadTotal(classifier, static_cast<double>(std::accumulate(spreads.begin(), spreads.end(), 0)) / spreads.size());
	repBuilder->addClassifiedCorrectTotal(classifier, classifiedCorrectlyTotal);
	repBuilder->addClassifiedIncorrectTotal(classifier, classifiedIncorrectlyTotal);
	repBuilder->addTotalTestObjectCount(classifier, classifiedCorrectlyTotal + classifiedIncorrectlyTotal);
}

void CorrectClassifierTester::notifyStartedTraining(Classifier* c)
{

}

void CorrectClassifierTester::notifyIteration(Classifier* c, unsigned int i)
{

}