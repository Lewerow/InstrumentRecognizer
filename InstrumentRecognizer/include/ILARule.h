#ifndef ILA_RULE_H_Djwioqdfj89w3fj49rfjdkcnerigvnirwe9fjiowjfnerifuwejdiqwodjqwijdiwefncdjksnfweuifuirhg4
#define ILA_RULE_H_Djwioqdfj89w3fj49rfjdkcnerigvnirwe9fjiowjfnerifuwejdiqwodjqwijdiwefncdjksnfweuifuirhg4

#include <ILADescription.h>


class ILARule
{
public:
    ILARule(const ClassName&, const ILADescription&);

    bool matches(const DiscretizedObjectDescription& desc) const;

	ClassName name() const;

private:
    ILADescription description;
    ClassName result;
};

#endif