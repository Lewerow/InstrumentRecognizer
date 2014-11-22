#pragma once

#include "common_defs.h"

#include <boost/heap/binomial_heap.hpp>
#include <functional>

typedef std::pair<ClassName, double> SingleClassifierResult;

template<>
struct std::greater<SingleClassifierResult>
{
	bool operator() (const SingleClassifierResult& lhs, const SingleClassifierResult& rhs) const
	{
		return lhs.second > rhs.second;
	}
};

template<>
struct std::less<SingleClassifierResult>
{
	bool operator() (const SingleClassifierResult& lhs, const SingleClassifierResult& rhs) const 
	{
		return lhs.second < rhs.second;
	}
};

typedef boost::heap::binomial_heap<SingleClassifierResult, boost::heap::compare<std::less<SingleClassifierResult> > > 
    ClassifierResults;

ClassifierResults singleResultClass(const ClassName& name);