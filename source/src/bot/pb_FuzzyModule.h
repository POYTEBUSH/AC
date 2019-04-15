#pragma once
#include "pb_FuzzyVariable.h"
#include "pb_FuzzyRule.h"
#include "pb_FuzzyTerm.h"

#include <map>
#include <vector>

const int CENTROID_MAX_SAMPLES = 15;

class pb_FuzzyModule
{
public:
	pb_FuzzyModule() {};
	~pb_FuzzyModule();

	pb_FuzzyVariable& CreateFuzzyVariable(const std::string& name);

	void AddRule(pb_FuzzyTerm& atecedent, pb_FuzzyTerm& concequence);

	double Fuzzify(const std::string& name, double val);
	double DeFuzzify(const std::string& name);

private:

	std::map<std::string, pb_FuzzyVariable*> mVariables;

	std::vector<pb_FuzzyRule*> mFuzzyRules;

	void ZeroConsequence();

};

