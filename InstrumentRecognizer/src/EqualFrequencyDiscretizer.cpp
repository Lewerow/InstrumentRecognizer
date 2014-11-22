#include <EqualFrequencyDiscretizer.h>

#include <algorithm>

EqualFrequencyDiscretizer::EqualFrequencyDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum) : Discretizer(maxParts, base, attributeNum)
{}

void EqualFrequencyDiscretizer::teach()
{
	std::vector<Descriptor> attributeValues;
	attributeValues.reserve(mBase.size() * mBase.begin()->second.size());

	for (auto& k : mBase)
	{
		for (auto& objects : k.second)
			attributeValues.push_back(objects[mAttribute]);
	}

	std::sort(attributeValues.begin(), attributeValues.end());

	std::size_t elemsPerClass = attributeValues.size() / mParts;
	elemsPerClass = (elemsPerClass > 0) ? elemsPerClass : 1;
	std::size_t nextClassFrequencyThreshold = elemsPerClass;
	for (std::size_t i = 0; i < attributeValues.size(); ++i)
	{
		if (i >= nextClassFrequencyThreshold && (thresholds.empty() || attributeValues[i] > thresholds.back()))
		{
            Descriptor thres = attributeValues[i];
            if (i + 1 < attributeValues.size())
                thres = (thres + attributeValues[i+1]) / 2;

			thresholds.push_back(thres);
			nextClassFrequencyThreshold = i + elemsPerClass;
		}
	}
}

Discretized EqualFrequencyDiscretizer::discretize(Descriptor value)
{
	for (Discretized d = 0; d < thresholds.size(); ++d)
	{
		if (thresholds[d] > value)
			return d;
	}

	return thresholds.size();
}

std::unique_ptr<EqualFrequencyDiscretizer> EqualFrequencyDiscretizer::creator(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum)
{
	return std::make_unique<EqualFrequencyDiscretizer>(maxParts, base, attributeNum);
}