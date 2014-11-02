#include <ILAClassifier.h>
#include <ClassifierVisitor.h>
#include <EqualSizeDiscretizer.h>

namespace
{
    ClassifierResults singleResultClass(const ClassName& name)
    {
        ClassifierResults res;
        res.emplace(name, 1);
        return res;
    }
}

ILAClassifier* ILAClassifier::Builder::build()
{
	return new ILAClassifier;
}

ILAClassifier* ILAClassifier::Builder::build(const std::string&)
{
	return new ILAClassifier;
}

ILAClassifier* ILAClassifier::Builder::build(const XMLNode&)
{
	return new ILAClassifier;
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
	return new Builder;
}

ClassName ILAClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

ClassifierResults ILAClassifier::doCalculation(const ObjectDescription& desc) const
{
    DiscretizedObjectDescription obj(discretizeSingleObject(desc));

    boost::optional<ClassName> cls;
    for (auto& r : rules)
    {
        cls = r.classify(obj);
        if (cls.is_initialized())
            return singleResultClass(cls.get());
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
    DiscretizedClassDescription currentClass = discretizedBase.begin()->second;
    ClassName currentClassName = discretizedBase.begin()->first;
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
		discretizers.push_back(std::make_unique<EqualSizeDiscretizer>(10, descriptionBase, i));

    for (auto& k : descriptionBase)
	{
		DiscretizedClassDescription d;
		for (auto& objectDescription : k.second)
            d.push_back(discretizeSingleObject(objectDescription));

		discretizedBase.insert(std::make_pair(k.first, d));
	}
}