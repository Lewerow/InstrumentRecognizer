#ifndef ATTRIBUTE_COMBINATIONS_H_mfioermgu45ingruijnckqwljd8923jf3uhrejknedjio23fj34f34
#define ATTRIBUTE_COMBINATIONS_H_mfioermgu45ingruijnckqwljd8923jf3uhrejknedjio23fj34f34

#include <vector>

class AttributeCombinations
{
public:
    AttributeCombinations(std::size_t count);

    std::vector<unsigned int> getNextAttributeSet();

private:
    std::size_t attributeCount;
    std::size_t currentCombinationSize;
    std::vector<unsigned int> previousAttributeSet;
};

#endif