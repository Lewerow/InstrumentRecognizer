#include <ILARule.h>

namespace
{
    bool description_matches(const ILADescription& rule, const DiscretizedObjectDescription& classified)
    {
        bool matches = true;
        IR_ASSERT(rule.size() == classified.size(), "Discretized objects must be same that ones used during teaching");

        for (std::size_t i = 0; i < rule.size(); ++i)
            matches = matches && (!rule[i] || (rule[i].get() == classified[i]));
    
        return matches;
    }
}

ILARule::ILARule(const ClassName& cls, const ILADescription& desc) : description(desc), result(cls)
{}

boost::optional<ClassName> ILARule::classify(const DiscretizedObjectDescription& desc) const
{
    if (description_matches(description, desc))
        return result;

    return boost::none;
}