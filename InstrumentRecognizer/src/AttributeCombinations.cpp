#include <AttributeCombinations.h>
#include <numeric>



AttributeCombinations::AttributeCombinations(std::size_t count) : attributeCount(count), currentCombinationSize(0)
{
    previousAttributeSet.reserve(attributeCount);
}

std::vector<unsigned int> AttributeCombinations::getNextAttributeSet()
{
    while (!previousAttributeSet.empty() && ((attributeCount - (++previousAttributeSet.back())) <= (currentCombinationSize - previousAttributeSet.size())))
        previousAttributeSet.pop_back();

    if (previousAttributeSet.empty() && currentCombinationSize <= attributeCount)
    {
        if(++currentCombinationSize <= attributeCount)
            previousAttributeSet.push_back(0);
    }

    if (previousAttributeSet.size() < currentCombinationSize && currentCombinationSize <= attributeCount)
    {
        std::size_t previousSize = previousAttributeSet.size();
        previousAttributeSet.resize(currentCombinationSize);
        std::iota(previousAttributeSet.end() - (currentCombinationSize - previousSize), previousAttributeSet.end(), previousAttributeSet[previousSize - 1] + 1);
    }

    return previousAttributeSet;
}