#include <ILARule.h>

ILARule::ILARule(const ClassName& cls, const ILADescription& desc) : description(desc), result(cls)
{}

ClassName ILARule::name() const
{
	return result;
}

bool ILARule::matches(const DiscretizedObjectDescription& desc) const
{
	bool matches = true;
	IR_ASSERT(description.size() == desc.size(), "Discretized objects must be same that ones used during teaching");

	for (std::size_t i = 0; i < description.size(); ++i)
		matches = matches && (!description[i] || (description[i].get() == desc[i]));

	return matches;
}