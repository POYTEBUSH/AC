#include "cube.h"
#include "pb_FuzzyTerm.h"


pb_FuzzyTerm::pb_FuzzyTerm()
{
}


pb_FuzzyTerm::~pb_FuzzyTerm()
{
}

pb_FuzzyTermGroup::pb_FuzzyTermGroup(pb_FuzzyTerm * op1, pb_FuzzyTerm * op2, pb_FuzzyTerm * op3, pb_FuzzyTerm * op4)
{
	mTerms.clear();
	 
	if (op1 != nullptr)
		mTerms.push_back(op1);

	if (op2 != nullptr)
		mTerms.push_back(op2);

	if (op3 != nullptr)
		mTerms.push_back(op3);

	if (op4 != nullptr)
		mTerms.push_back(op4);
}

double pb_FuzzyTermGroup::GetDegreeOfMembership() const
{
	double minVal = 1e4;
	for (auto ts : mTerms)
	{
		minVal = min(minVal, ts->GetDegreeOfMembership());
	}

	return minVal;
}

void pb_FuzzyTermGroup::ClearDegreeOfMemebership()
{
	for (auto ts : mTerms)
	{
		ts->ClearDegreeOfMemebership();
	}
}

void pb_FuzzyTermGroup::ORWithDegreeOfMemebership(double val)
{
	for (auto ts : mTerms)
	{
		ts->ORWithDegreeOfMemebership(val);
	}
}
