#include "cube.h"
#include "pb_FuzzyTerm.h"

pb_FuzzyTermAND::pb_FuzzyTermAND(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2)
{
	mTerms.clear();

	mTerms.push_back(op1.Clone());
	mTerms.push_back(op2.Clone());
}
pb_FuzzyTermAND::pb_FuzzyTermAND(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2, pb_FuzzyTerm& op3)
{
	mTerms.clear();

	mTerms.push_back(op1.Clone());
	mTerms.push_back(op2.Clone());
	mTerms.push_back(op3.Clone());
}

pb_FuzzyTermAND::pb_FuzzyTermAND(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2, pb_FuzzyTerm& op3, pb_FuzzyTerm& op4)
{
	mTerms.clear();

	mTerms.push_back(op1.Clone());
	mTerms.push_back(op2.Clone());
	mTerms.push_back(op3.Clone());
	mTerms.push_back(op4.Clone());
}

double pb_FuzzyTermAND::GetDegreeOfMembership() const
{
	double minVal = 1e4;
	for (auto ts : mTerms)
	{
		minVal = min(minVal, ts->GetDegreeOfMembership());
	}

	return minVal;
}

void pb_FuzzyTermAND::ClearDegreeOfMemebership()
{
	for (auto ts : mTerms)
	{
		ts->ClearDegreeOfMemebership();
	}
}

void pb_FuzzyTermAND::ORWithDegreeOfMemebership(double val)
{
	for (auto ts : mTerms)
	{
		ts->ORWithDegreeOfMemebership(val);
	}
}

pb_FuzzyTermOR::pb_FuzzyTermOR(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2)
{
	mTerms.clear();

	mTerms.push_back(op1.Clone());
	mTerms.push_back(op2.Clone());
}
pb_FuzzyTermOR::pb_FuzzyTermOR(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2, pb_FuzzyTerm& op3)
{
	mTerms.clear();

	mTerms.push_back(op1.Clone());
	mTerms.push_back(op2.Clone());
	mTerms.push_back(op3.Clone());
}

pb_FuzzyTermOR::pb_FuzzyTermOR(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2, pb_FuzzyTerm& op3, pb_FuzzyTerm& op4)
{
	mTerms.clear();

	mTerms.push_back(op1.Clone());
	mTerms.push_back(op2.Clone());
	mTerms.push_back(op3.Clone());
	mTerms.push_back(op4.Clone());
}

double pb_FuzzyTermOR::GetDegreeOfMembership() const
{
	double maxVal = -1e4;
	for (auto ts : mTerms)
	{
		maxVal = max(maxVal, ts->GetDegreeOfMembership());
	}

	return maxVal;
}

void pb_FuzzyTermOR::ClearDegreeOfMemebership()
{
	for (auto ts : mTerms)
	{
		ts->ClearDegreeOfMemebership();
	}
}

void pb_FuzzyTermOR::ORWithDegreeOfMemebership(double val)
{
	for (auto ts : mTerms)
	{
		ts->ORWithDegreeOfMemebership(val);
	}
}
