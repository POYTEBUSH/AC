#include "cube.h"
#include "pb_FuzzyModule.h"

#include <assert.h>

void pb_FuzzyModule::AddRule(pb_FuzzyTerm & atecedent, pb_FuzzyTerm & concequence)
{
	mFuzzyRules.push_back(new pb_FuzzyRule(atecedent, concequence));
}

pb_FuzzyVariable & pb_FuzzyModule::CreateFuzzyVariable(const std::string & name)
{
	mVariables[name] = new pb_FuzzyVariable();
	return *mVariables[name];
}

void pb_FuzzyModule::Fuzzify(const std::string & name, double val)
{
	mVariables[name]->Fuzzify(val);
}

double pb_FuzzyModule::DeFuzzifyCentroid(const std::string & name, int samples)
{
	assert(mVariables.find(name) != end(mVariables));

	for (auto r : mFuzzyRules)
	{
		r->Calculate();
	}

	return mVariables[name]->DeFuzzifyCentroid(samples);
}

double pb_FuzzyModule::DeFuzzifyMaxAv(const std::string & name)
{
	assert(mVariables.find(name) != end(mVariables));

	for (auto r : mFuzzyRules)
	{
		r->Calculate();
	}

	return mVariables[name]->DeFuzzifyMaxAv();
}

void pb_FuzzyModule::ZeroConsequence()
{
	for (auto r : mFuzzyRules)
	{
		r->ZeroConsequnceConfidence();
	}
}
