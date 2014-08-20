#include <memory>
#include <shark/Algorithms/NearestNeighbors/TreeNearestNeighbors.h>
#include <shark/Algorithms/NearestNeighbors/SimpleNearestNeighbors.h>
#include <shark/Models/Kernels/LinearKernel.h>
#include <shark/Models/Trees/KDTree.h>
#include <shark/Models/Trees/KHCTree.h>

#include "NearestNeighborClassifier.h"

class StandardTreeStrategy : public NearestNeighborClassifier::Strategy
{
public:

	StandardTreeStrategy(unsigned int neighbors) : Strategy(neighbors), data(nullptr), algorithm(nullptr)
	{}

	StandardTreeStrategy(unsigned int neighbors, const Input& in) : Strategy(neighbors)
	{
		setInputData(in);
	}

	virtual ~StandardTreeStrategy(){}

	virtual void setInputData(const Input& in)
	{
		data.reset(new shark::KDTree<Input::InputType>(in.inputs()));
		algorithm.reset(new shark::TreeNearestNeighbors<Input::InputType, Input::LabelType>(in, data.get()));
	}

	virtual Implementation* getAlgorithm()
	{
		IR_ASSERT(algorithm.get() != nullptr, "Algorithm cannot be NULL if you want to use it! StandardTreeStrategy");
		return algorithm.get();
	}
	
	virtual Strategy* clone()
	{
		IR_ASSERT(data.get() == nullptr, "Cannot clone strategy with defined data! StandardTreeStrategy");
		return new StandardTreeStrategy(numberOfNeighbors);
	};

private:
	std::unique_ptr<shark::KDTree<Input::InputType> > data;
	std::unique_ptr<shark::TreeNearestNeighbors<Input::InputType, Input::LabelType> > algorithm; // may become std::unique_ptr<Implementation> after fixes in shark
	// until then it'll stay the same, because otherwise it'll leak
};
/*
class TreeLinearKernelStrategy : public NearestNeighborClassifier::Strategy
{
public:

	TreeLinearKernelStrategy(unsigned int neighbors) : Strategy(neighbors), data(nullptr), algorithm(nullptr)
	{}


	TreeLinearKernelStrategy(unsigned int neighbors, const Input& in) : Strategy(neighbors)
	{
		setInputData(in);
	}

	virtual ~TreeLinearKernelStrategy(){}

	virtual void setInputData(const Input& in)
	{
		data.reset(new shark::KHCTree<Input::InputType>(in.inputs(), &kernel));
		algorithm.reset(new shark::TreeNearestNeighbors<Input::InputType, Input::LabelType>(in, data.get()));
	}

	virtual Implementation* getAlgorithm()
	{
		IR_ASSERT(algorithm.get() != nullptr, "Algorithm cannot be NULL if you want to use it! LinearKernelStrategy");
		return algorithm.get();
	}
	
	virtual Strategy* clone()
	{
		IR_ASSERT(data.get() == nullptr, "Cannot clone strategy with defined data! LinearKernelStrategy");
		return new TreeLinearKernelStrategy(numberOfNeighbors);
	};

private:
	const shark::LinearKernel<Input::InputType> kernel;
	std::unique_ptr<shark::KHCTree<Input::InputType> > data;
	std::unique_ptr<shark::TreeNearestNeighbors<Input::InputType, Input::LabelType> > algorithm; // may become std::unique_ptr<Implementation> after fixes in shark
	// until then it'll stay the same, because otherwise it'll leak
};
*/
class SimpleKernelStrategy : public NearestNeighborClassifier::Strategy
{
public:

	SimpleKernelStrategy(unsigned int neighbors) : Strategy(neighbors), kernel(new shark::LinearKernel<>()), algorithm(nullptr)
	{}

	SimpleKernelStrategy(unsigned int neighbors, const Input& in) : Strategy(neighbors), kernel(new shark::LinearKernel<>())
	{
		setInputData(in);
	}

	virtual ~SimpleKernelStrategy(){}

	virtual void setInputData(const Input& in)
	{
		algorithm.reset(new shark::SimpleNearestNeighbors<Input::InputType, Input::LabelType>(in, kernel.get()));
	}

	virtual Implementation* getAlgorithm()
	{
		IR_ASSERT(algorithm.get() != nullptr, "Algorithm cannot be NULL if you want to use it! LinearKernelStrategy");
		return algorithm.get();
	}

	virtual Strategy* clone()
	{
		IR_ASSERT(algorithm.get() == nullptr, "Cannot clone strategy with defined data! LinearKernelStrategy");
		return new SimpleKernelStrategy(numberOfNeighbors);
	};

	virtual unsigned int getNumberOfNeighbors()
	{
		return numberOfNeighbors;
	}

private:
	std::unique_ptr<shark::LinearKernel<> > kernel;
	std::unique_ptr<shark::SimpleNearestNeighbors<Input::InputType, Input::LabelType> > algorithm; // may become std::unique_ptr<Implementation> after fixes in shark
	// until then it'll stay the same, because otherwise it'll leak
};