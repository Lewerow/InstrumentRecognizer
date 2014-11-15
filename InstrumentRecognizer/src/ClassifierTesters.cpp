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
	if (!reportBuilder)
		return;

	auto repBuilder = reportBuilder.get();

	const ClassDescriptionBase& desc = descriptionDBManager->getTestDescriptions();

	std::map<ClassName, unsigned int> classifiedCorrectlyTotal;
	unsigned int classifiedIncorrectlyTotal = 0;
	std::vector<unsigned int> spreads;
	std::map<ClassName, unsigned int> classificationCount;

	for (const auto& cls : desc)
	{
		std::unordered_set<ClassName> spreadCounter;

		unsigned int classifiedCorrectlySingle = 0;
		unsigned int classifiedIncorrectlySingle = 0;

		for (const auto& obj : cls.second)
		{
			ClassName result = classifier->classify(obj);
			repBuilder->addClassificationResult(classifier, cls.first, obj, result);
			++classificationCount[result];

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

		classifiedCorrectlyTotal[cls.first] += classifiedCorrectlySingle;
		classifiedIncorrectlyTotal += classifiedIncorrectlySingle;
		spreads.push_back(spreadCounter.size());
	}

	unsigned int totalCorrect = std::accumulate(classifiedCorrectlyTotal.begin(), classifiedCorrectlyTotal.end(), 0, [](unsigned int a, const std::pair<ClassName, unsigned int>& p){return a + p.second; });

	std::vector<double> precisions;
	std::vector<double> recalls;
	std::vector<double> accurracies;
	std::vector<double> fmeasures;
	for (auto& c : classificationCount)
	{
		if (c.first.empty())
			continue;

		double accurracy = totalCorrect / ((double)(totalCorrect + classifiedIncorrectlyTotal));
		repBuilder->addAccurracySingle(classifier, c.first, accurracy);
		accurracies.push_back(accurracy);

        double precision = 0;
        if (c.second > 0)
		    precision = classifiedCorrectlyTotal[c.first] / ((double)(c.second));
		
        repBuilder->addPrecisionSingle(classifier, c.first, precision);
		precisions.push_back(precision);

        double recall = 0;
        if(desc.count(c.first) && !desc.at(c.first).empty())
		    recall = classifiedCorrectlyTotal[c.first] / ((double)(desc.at(c.first).size()));
		
        repBuilder->addRecallSingle(classifier, c.first, recall);
		recalls.push_back(recall);

		double fmeasure = 0;;
		if (precision + recall > 0.000001)
            fmeasure = 2 * (precision * recall) / (precision + recall);

		repBuilder->addFMeasureSingle(classifier, c.first, fmeasure);
		fmeasures.push_back(fmeasure);
	}


	IR_ASSERT(!spreads.empty(), "How in hell we got here, if there are no training objects??");

	repBuilder->addPrecisionTotal(classifier, std::accumulate(precisions.begin(), precisions.end(), 0.0) / precisions.size());
	repBuilder->addAccurracyTotal(classifier, std::accumulate(accurracies.begin(), accurracies.end(), 0.0) / accurracies.size());
	repBuilder->addRecallTotal(classifier, std::accumulate(recalls.begin(), recalls.end(), 0.0) / recalls.size());
	repBuilder->addFMeasureTotal(classifier, std::accumulate(fmeasures.begin(), fmeasures.end(), 0.0) / fmeasures.size());
	repBuilder->addSpreadTotal(classifier, static_cast<double>(std::accumulate(spreads.begin(), spreads.end(), 0.0)) / spreads.size());
	repBuilder->addClassifiedCorrectTotal(classifier, totalCorrect);
	repBuilder->addClassifiedIncorrectTotal(classifier, classifiedIncorrectlyTotal);
	repBuilder->addTotalTestObjectCount(classifier, totalCorrect + classifiedIncorrectlyTotal);
}

void CorrectClassifierTester::notifyStartedTraining(Classifier* c)
{

}

void CorrectClassifierTester::notifyIteration(Classifier* c, unsigned int i)
{

}