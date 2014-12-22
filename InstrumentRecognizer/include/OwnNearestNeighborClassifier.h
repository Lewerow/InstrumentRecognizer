#pragma once

#include "Classifier.h"

#include <type_traits>
#include <memory>

double euclidDistance(const ObjectDescription& o1, const ObjectDescription& o2);
double manhattanDistance(const ObjectDescription& o1, const ObjectDescription& o2);
double minkowski3Distance(const ObjectDescription& o1, const ObjectDescription& o2);

class Normalizer;

class OwnNearestNeighborClassifier : public Classifier
{
public:
	typedef double(*DistanceMetric)(const ObjectDescription& o1, const ObjectDescription& o2);
	typedef ClassName(*VotingMethod)(std::vector<std::pair<ClassName, ObjectDescription> >&, const ObjectDescription& in, DistanceMetric);

	static ClassName majorityVoting(std::vector<std::pair<ClassName, ObjectDescription> >&, const ObjectDescription& in, DistanceMetric);
	static ClassName weightedVoting(std::vector<std::pair<ClassName, ObjectDescription> >&, const ObjectDescription& in, DistanceMetric);
	static ClassName dualWeightedVoting(std::vector<std::pair<ClassName, ObjectDescription> >&, const ObjectDescription& in, DistanceMetric);

	struct Strategy
	{
		std::size_t k;
		VotingMethod voting;
		DistanceMetric metric;
	};

	class Builder : public Classifier::Builder
	{
	public:

		Builder(Strategy strategy);

		virtual OwnNearestNeighborClassifier* build();
		virtual OwnNearestNeighborClassifier* build(const std::string&);
		virtual OwnNearestNeighborClassifier* build(const XMLNode&);
		virtual std::unique_ptr<XMLNode> dismantleToXML(std::shared_ptr<Classifier>);
		virtual std::string dismantleToText(std::shared_ptr<Classifier>);

	private:
		Strategy strategy;
	};

	OwnNearestNeighborClassifier(Strategy s );

	virtual ~OwnNearestNeighborClassifier();

	virtual void accept(ClassifierVisitor* visitor);

private:
	virtual Builder* doMakeBuilder();
	virtual ClassName doClassification(const ClassifierResults&) const;
	virtual ClassifierResults doCalculation(const ObjectDescription&) const;
	virtual void doRun();
	virtual void doStop();

	Strategy strategy;
	std::unique_ptr<Normalizer> normalizer;
	std::vector<std::pair<ClassName, ObjectDescription> > dataPoints;
};