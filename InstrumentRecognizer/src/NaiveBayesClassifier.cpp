#include <NaiveBayesClassifier.h>
#include <ClassifierVisitor.h>
#include <Discretizer.h>

#include <numeric>

NaiveBayesClassifier::NaiveBayesClassifier(std::shared_ptr<DiscretizerFactory> disc) : discretizerFactory(disc)
{}

NaiveBayesClassifier::Builder::Builder(DiscretizerFactory::Creator discretizer, std::size_t defaultClassCount)
{
	factory = std::make_shared<DiscretizerFactory>(defaultClassCount);
	factory->registerCreator("default", discretizer);
	factory->setDefault("default");
}

NaiveBayesClassifier* NaiveBayesClassifier::Builder::build()
{
	return new NaiveBayesClassifier(factory);
}

NaiveBayesClassifier* NaiveBayesClassifier::Builder::build(const std::string&)
{
	return new NaiveBayesClassifier(factory);
}

NaiveBayesClassifier* NaiveBayesClassifier::Builder::build(const XMLNode&)
{
	return new NaiveBayesClassifier(factory);
}

std::unique_ptr<XMLNode> NaiveBayesClassifier::Builder::dismantleToXML(std::shared_ptr<Classifier>)
{
	// not supported
	return std::unique_ptr<XMLNode>(nullptr);
}

std::string NaiveBayesClassifier::Builder::dismantleToText(std::shared_ptr<Classifier>)
{
	// not supported
	return std::string();
}

NaiveBayesClassifier::Builder* NaiveBayesClassifier::doMakeBuilder()
{
	// not supported
	return nullptr;
}

ClassName NaiveBayesClassifier::doClassification(const ClassifierResults& res) const
{
	IR_ASSERT(!res.empty(), "Cannot classify empty result set!");
	return res.top().first;
}

double NaiveBayesClassifier::probablilityFor(const ClassName& cls, const DiscretizedObjectDescription& desc) const
{
	double prob = 1;
	for (std::size_t att = 0; att < desc.size(); ++att)
	{
		auto value = probabilities[att].find(desc[att]);
		if (value == probabilities[att].end()) // means: attribute never had such value in training data. Possible for some discretizations
			return 0;

		auto cls_prob = value->second.find(cls);
		if (cls_prob == value->second.end()) // means: this class never had this attribute value
			return 0;

		prob *= cls_prob->second;
	}

	return prob;
}
ClassifierResults NaiveBayesClassifier::doCalculation(const ObjectDescription& desc) const
{
	auto discretized = discretizeSingleObject(desc);

	ClassifierResults res;
	for (auto& cls : discretizedBase)
		res.push(SingleClassifierResult(cls.first, probablilityFor(cls.first, discretized)));

	return res;
}

void NaiveBayesClassifier::accept(ClassifierVisitor* visitor)
{
	visitor->visit(this);
}

void NaiveBayesClassifier::doRun()
{
	discretizeAll();
	teach();
}

void NaiveBayesClassifier::doStop()
{}

void NaiveBayesClassifier::teach()
{
	std::size_t totalObjects = 0;
	for (auto& i : discretizedBase)
		totalObjects += i.second.size();

	probabilities.resize(discretizedBase.begin()->second[0].size());

	for (auto& cls : discretizedBase)
	{
		for (auto& obj : cls.second)
		{
			for (std::size_t att = 0; att < obj.size(); ++att)
				probabilities[att][obj[att]][cls.first] += (double(1)) / totalObjects;
		}
	}
}

DiscretizedObjectDescription NaiveBayesClassifier::discretizeSingleObject(const ObjectDescription& obj) const
{
	DiscretizedObjectDescription desc;
	for (std::size_t i = 0; i < obj.size(); ++i)
		desc.push_back(discretizers[i]->discretize(obj[i]));

	return desc;
}

void NaiveBayesClassifier::discretizeAll()
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