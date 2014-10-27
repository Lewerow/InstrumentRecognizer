#include <TrivialDiscretizer.h>

TrivialDiscretizer::TrivialDiscretizer(std::size_t maxParts, const ClassDescriptionBase& base, std::size_t attributeNum) : mParts(maxParts), mBase(base), mAttribute(attributeNum)
{
	Descriptor min = base.begin()->second[0][mAttribute];
	Descriptor max = base.begin()->second[0][mAttribute];

	for (auto& k : base)
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

	Descriptor diff = max - min;	
	for (std::size_t i = 0; i < mParts - 1; ++i)
		thresholds.push_back(min + diff);
}

Discretized TrivialDiscretizer::discretize(Descriptor value)
{
	for (Discretized d = 0; d < thresholds.size(); ++d)
	{
		if (thresholds[d] > value)
			return d;
	}

	return thresholds.size();
}