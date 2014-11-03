#include <ILAClassifier.h>
#include <ClassifierVisitor.h>
#include <EqualSizeDiscretizer.h>
#include <AttributeCombinations.h>

namespace
{
    ClassifierResults singleResultClass(const ClassName& name)
    {
        ClassifierResults res;
        res.emplace(name, 1);
        return res;
    }

	ILADescription extractDescription(const std::vector<unsigned int>& importantFields, const DiscretizedObjectDescription& desc)
	{
		ILADescription res(desc.size(), boost::none);

		for (auto f : importantFields)
			res[f] = desc[f];

		return res;
	}

	bool operator<(const ILADescription& lhs, const ILADescription& rhs)
	{
		IR_ASSERT(lhs.size() == rhs.size(), "Compared ILADescriptions must have same sizes");

		for (std::size_t i = 0; i < lhs.size(); ++i)
		{
			if (!lhs[i].is_initialized() && rhs[i].is_initialized())
				return true;

			if (lhs[i].is_initialized() && !rhs[i].is_initialized())
				return false;

			if (lhs[i] == rhs[i])
				continue;

			return lhs[i] < rhs[i];
		}

		return false;
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
	std::map<ILADescription, std::map<ClassName, int> > descriptionCounts;

	DiscretizedClassDescriptionBase notYetDiscretized(discretizedBase);
    for (auto usedAttributes = comb.getNextAttributeSet(); !usedAttributes.empty() && !notYetDiscretized.empty(); usedAttributes = comb.getNextAttributeSet())
	{
		for (auto& cls : discretizedBase)
		{
			for (auto& obj : cls.second)
				++descriptionCounts[extractDescription(usedAttributes, obj)][cls.first];
		}

		for (auto& desc : descriptionCounts)
		{
			if (desc.second.size() == 1)
			{
				ClassName relevantClass = desc.second.begin()->first;
				ILARule rule(relevantClass, desc.first);
				rules.push_back(rule);

				if (notYetDiscretized.count(relevantClass) == 0)
					continue;

				for (auto objIter = notYetDiscretized[relevantClass].begin();;)
				{
					if (rule.matches(*objIter))
						objIter = notYetDiscretized[relevantClass].erase(objIter);
					else
						++objIter;

					if (objIter == notYetDiscretized[relevantClass].end())
						break;
				}

				if (notYetDiscretized[relevantClass].empty())
					notYetDiscretized.erase(relevantClass);
			}
		}
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