#include "cube.h"
#include "pb_FuzzyModule.h"

pb_FuzzyModule::pb_FuzzyModule()
{
}


pb_FuzzyModule::~pb_FuzzyModule()
{
}

void pb_FuzzyModule::AddRule(pb_FuzzyTerm & atecedent, pb_FuzzyTerm & concequence)
{
	mFuzzyRules.push_back(new pb_FuzzyRule(atecedent, concequence));
}
