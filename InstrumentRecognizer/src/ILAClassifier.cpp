#include <ILAClassifier.h>
#include <ClassifierVisitor.h>
#include <AttributeCombinations.h>
#include <Discretizer.h>

namespace
{
    ILADescription extractDescription(const std::vector<unsigned int>& importantFields, const DiscretizedObjectDescription& desc)
    {
        ILADescription res;
        res.reserve(importantFields.size());

        for (auto f : importantFields)
            res.push_back(std::make_pair(f, desc[f]));

        return res;
    }
    
    bool operator<(const ILADescription& lhs, const ILADescription& rhs)
	{
        if (lhs.size() != rhs.size())
            return lhs.size() < rhs.size();

		for (std::size_t i = 0; i < lhs.size(); ++i)
		{
			if (lhs[i].first != rhs[i].first)
				return lhs[i].first < rhs[i].first;
            
            if (lhs[i].second != rhs[i].second)
				return lhs[i].second < rhs[i].second;
		}

		return false;
	}

    std::map<ILADescription, std::pair<ClassName, int> > calculateDescriptionCounts(const std::vector<std::vector<unsigned int> >& sameSizeCombinations, 
        const DiscretizedClassDescriptionBase& discretizedBase)
    {
        std::map<ILADescription, std::pair<ClassName, int> > descriptionCounts;
        for (auto& usedAttributes : sameSizeCombinations)
        {
            for (auto& cls : discretizedBase)
            {
                for (auto& obj : cls.second)
                {
                    auto desc = extractDescription(usedAttributes, obj);
                    auto it = descriptionCounts.find(desc);
                    if (it == descriptionCounts.end())
                        descriptionCounts.insert(std::make_pair(desc, std::make_pair(cls.first, 1)));
                    else if (!it->second.first.empty())
                    {
                        if (it->second.first != cls.first)
                            it->second.first.clear();
                        else
                            ++(it->second.second);
                    }
                }
            }
        }

        for (auto it = descriptionCounts.begin(); it != descriptionCounts.end();)
        {
            if (it->second.first.empty())
                it = descriptionCounts.erase(it);
            else
                ++it;
        }

        return descriptionCounts;
    }

    std::map<ILADescription, std::pair<ClassName, int> > recalculateDescriptionCounts(std::map<ILADescription, std::pair<ClassName, int> >& descriptionCounts,
        const DiscretizedClassDescriptionBase& notYetDiscretized)
    {
        for (auto& d : descriptionCounts)
        {
            d.second.second = 0;

            auto cls = notYetDiscretized.find(d.second.first);
            if (cls == notYetDiscretized.end())
                continue;

            ILARule rule(d.second.first, d.first);
            for (auto& obj : cls->second)
            {
                if (rule.matches(obj))
                    ++d.second.second;
            }
        }

        for (auto it = descriptionCounts.begin(); it != descriptionCounts.end();)
        {
            if (it->second.second == 0)
                it = descriptionCounts.erase(it);
            else
                ++it;
        }

        return descriptionCounts;
    }

    std::map<ClassName, ILADescription> pickMaxCombinations(std::map<ILADescription, std::pair<ClassName, int> >& allCombinations)
    {
        std::map<ClassName, ILADescription> maxCombinations;
        for (auto& comb : allCombinations)
        {
            if (allCombinations[maxCombinations[comb.second.first]].second < comb.second.second)
                maxCombinations[comb.second.first] = comb.first;
        }

        for (auto& comb: maxCombinations)
            allCombinations.erase(comb.second);

        return maxCombinations;
    }
}

ILAClassifier::ILAClassifier(std::shared_ptr<DiscretizerFactory> disc) : discretizerFactory(disc)
{}

ILAClassifier::Builder::Builder(DiscretizerFactory::Creator discretizer, std::size_t defaultClassCount)
{
	factory = std::make_shared<DiscretizerFactory>(defaultClassCount);
	factory->registerCreator("default", discretizer);
	factory->setDefault("default");
}

ILAClassifier* ILAClassifier::Builder::build()
{
	return new ILAClassifier(factory);
}

ILAClassifier* ILAClassifier::Builder::build(const std::string&)
{
	return new ILAClassifier(factory);
}

ILAClassifier* ILAClassifier::Builder::build(const XMLNode&)
{
	return new ILAClassifier(factory);
}

std::unique_ptr<XMLNode> ILAClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	// not supported
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string ILAClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	// not supported
	return std::string();
}

ILAClassifier::Builder* ILAClassifier::doMakeBuilder()
{
	// not supported
	return nullptr;
}

ClassName ILAClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

ClassifierResults ILAClassifier::doCalculation(const ObjectDescription& desc) const
{
    DiscretizedObjectDescription obj(discretizeSingleObject(desc));

    for (auto& r : rules)
    {
        if (r.matches(obj))
            return singleResultClass(r.name());
    }

    return singleResultClass("");
}

void ILAClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

void ILAClassifier::doRun()
{
	discretizeAll();
    teach();
}

void ILAClassifier::doStop()
{}

void ILAClassifier::teach()
{
	AttributeCombinations comb(discretizedBase.begin()->second.begin()->size());

	DiscretizedClassDescriptionBase notYetDiscretized(discretizedBase);
    std::vector<Discretized> nextCombination;
    while (!notYetDiscretized.empty())
    {
        std::vector<std::vector<Discretized> > sameSizeCombinations;
        do
        {
            nextCombination = comb.getNextAttributeSet();
            sameSizeCombinations.push_back(nextCombination);
        } while (!nextCombination.empty() && nextCombination.size() == sameSizeCombinations[0].size());

        for (auto descriptionCounts = calculateDescriptionCounts(sameSizeCombinations, discretizedBase);
            !descriptionCounts.empty(); )
        {
            descriptionCounts = recalculateDescriptionCounts(descriptionCounts, notYetDiscretized);
            if (descriptionCounts.empty())
                break;

            auto maxCombinations = pickMaxCombinations(descriptionCounts);
            for (auto& maxComb : maxCombinations)
            {
                ClassName relevantClass = maxComb.first;
                ILARule rule(relevantClass, maxComb.second);

                for (auto objIter = notYetDiscretized[relevantClass].begin(); objIter != notYetDiscretized[relevantClass].end();)
                {
                    if (rule.matches(*objIter))
                        objIter = notYetDiscretized[relevantClass].erase(objIter);
                    else
                        ++objIter;
                }

                rules.push_back(rule);

                if (notYetDiscretized[relevantClass].empty())
                    notYetDiscretized.erase(relevantClass);
            }
        }

        if (nextCombination.empty())
            break;   
    }
}

DiscretizedObjectDescription ILAClassifier::discretizeSingleObject(const ObjectDescription& obj) const
{
    DiscretizedObjectDescription desc;
    for (std::size_t i = 0; i < obj.size(); ++i)
        desc.push_back(discretizers[i]->discretize(obj[i]));

    return desc;
}

void ILAClassifier::discretizeAll()
{
	std::size_t attributeCount = descriptionBase.begin()->second[0].size();

	for (std::size_t i = 0; i < attributeCount; ++i)
	{
		discretizers.push_back(discretizerFactory->getDefaultDiscretizer(boost::none, descriptionBase, i));
		discretizers[i]->teach();
	}

    for (auto& k : descriptionBase)
	{
		DiscretizedClassDescription d;
		for (auto& objectDescription : k.second)
            d.push_back(discretizeSingleObject(objectDescription));

		discretizedBase.insert(std::make_pair(k.first, d));
	}
}