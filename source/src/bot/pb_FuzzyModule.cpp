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

pb_FuzzyVariable & pb_FuzzyModule::CreateFuzzyVariable(const std::string & name)
{
	mVariables[name] = new pb_FuzzyVariable();
	return *mVariables[name];
}

double pb_FuzzyModule::Fuzzify(const std::string & name, double val)
{
	mVariables[name]->Fuzzify(val);
}

double pb_FuzzyModule::DeFuzzify(const std::string & name)
{
	assert(mVariables.find(name) != end(mVariables));

	for (auto r : mFuzzyRules)
	{
		r->Calculate();
	}

	return mVariables[name]->DeFuzzify(10);
}

void pb_FuzzyModule::ZeroConsequence()
{
}
