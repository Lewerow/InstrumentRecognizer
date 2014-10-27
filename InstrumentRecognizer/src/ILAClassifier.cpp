#include <ILAClassifier.h>
#include <ClassifierVisitor.h>

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
	return ClassifierResults();
}

void ILAClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

void ILAClassifier::doRun()
{
	discretize();
}

void ILAClassifier::doStop()
{

}

void ILAClassifier::discretize()
{
	std::size_t attributeCount = descriptionBase.begin()->second[0].size();

	for (std::size_t i = 0; i < attributeCount; ++i)
		discretizers.push_back(TrivialDiscretizer(10, descriptionBase, i));

	for (auto& k : descriptionBase)
	{
		DiscretizedClassDescription d;
		for (auto& j : k.second)
		{
			DiscretizedObjectDescription desc;
			for (int i = 0; i < j.size(); ++i)
				desc.push_back(discretizers[i].discretize(j[i]));

			d.push_back(desc);
		}

		discretizedBase.insert(std::make_pair(k.first, d));
	}

}