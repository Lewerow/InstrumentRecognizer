#include <EqualFrequencyDiscretizer.h>

#include <algorithm>

EqualFrequencyDiscretizer::EqualFrequencyDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum) : Discretizer(maxParts, base, attributeNum)
{}

void EqualFrequencyDiscretizer::teach()
{
	std::vector<Descriptor> attributeValues;
	attributeValues.reserve(mBase.size() * mBase.begin()->second.size());

	Descriptor min = mBase.begin()->second[0][mAttribute];
	Descriptor max = mBase.begin()->second[0][mAttribute];

	for (auto& k : mBase)
	{
		for (auto& objects : k.second)
			attributeValues.push_back(objects[mAttribute]);
	}

	std::sort(attributeValues.begin(), attributeValues.end());

	std::size_t elemsPerClass = attributeValues.size() / mParts;
	std::size_t nextClassFrequencyThreshold = elemsPerClass;
	for (std::size_t i = 0; i < attributeValues.size(); ++i)
	{
		if (i == nextClassFrequencyThreshold)
		{
			thresholds.push_back((attributeValues[i] + attributeValues[i+1]) / 2);
			nextClassFrequencyThreshold += elemsPerClass;
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