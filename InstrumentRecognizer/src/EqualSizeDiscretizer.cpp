#include <EqualSizeDiscretizer.h>

EqualSizeDiscretizer::EqualSizeDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum) : Discretizer(maxParts, base, attributeNum)
{}

void EqualSizeDiscretizer::teach()
{
	Descriptor min = mBase.begin()->second[0][mAttribute];
	Descriptor max = mBase.begin()->second[0][mAttribute];

	for (auto& k : mBase)
	{
		for (auto& objects : k.second)
		{
			Descriptor m = objects[mAttribute];

			if (m < min)
				min = m;
			else if (m > max)
				max = m;
		}
	}

	Descriptor diff = (max - min) / mParts;
	for (std::size_t i = 1; i < mParts; ++i)
		thresholds.push_back(min + i*diff);
}

Discretized EqualSizeDiscretizer::discretize(Descriptor value)
{
	for (Discretized d = 0; d < thresholds.size(); ++d)
	{
		if (thresholds[d] > value)
			return d;
	}

	return thresholds.size();
}