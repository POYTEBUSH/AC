#pragma once

#include "pb_FuzzyTerm.h"

class pb_FuzzyRule
{

public:
	pb_FuzzyRule(pb_FuzzyTerm& ant, pb_FuzzyTerm& con) : mAtecedent(ant.Clone()), mConsequence(con.Clone()) {};
	~pb_FuzzyRule() { delete mAtecedent; delete mConsequence; };

	void ZeroConsequnceConfidence() { mConsequence->ClearDegreeOfMemebership(); }
	void Calculate()
	{
		mConsequence->ORWithDegreeOfMemebership(mAtecedent->GetDegreeOfMembership());
	}

private:

	pb_FuzzyTerm* mAtecedent;
	pb_FuzzyTerm* mConsequence;

	//Disable copying of rules
	pb_FuzzyRule(const pb_FuzzyRule&);
	pb_FuzzyRule& operator=(const pb_FuzzyRule&);
};

