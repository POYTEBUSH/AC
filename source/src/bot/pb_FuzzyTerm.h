#pragma once
#include "pb_FuzzySet.h"

#include <vector>

class pb_FuzzyTerm
{
public:
	pb_FuzzyTerm();

	virtual ~pb_FuzzyTerm() {};

	virtual pb_FuzzyTerm* Clone() const = 0;

	virtual double GetDegreeOfMembership() const = 0;

	virtual void ClearDegreeOfMemebership() = 0;

	virtual void ORWithDegreeOfMemebership(double val) = 0;
};

class pb_FzSet : public pb_FuzzyTerm
{

public:

	pb_FzSet(pb_FuzzySet& fs) :m_Set(fs) {}

	pb_FuzzyTerm* Clone()const { return new pb_FzSet(*this); }
	double     GetDegreeOfMembership()const { return m_Set.GetDegreeOfMembership(); }
	void       ClearDegreeOfMemebership() { m_Set.ResetDegreeOfMemebership(); }
	void       ORWithDegreeOfMemebership(double val) { m_Set.ORWithDOM(val); }

private:

	//a reference to the fuzzy set this proxy represents
	pb_FuzzySet & m_Set;
};

class pb_FuzzyTermGroup : public pb_FuzzyTerm
{
public:

	pb_FuzzyTermGroup(const pb_FuzzyTermGroup& fa)
	{
		for (auto fs : fa.mTerms)
		{
			mTerms.push_back(fs->Clone());
		}
	}
	pb_FuzzyTermGroup(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2);
	pb_FuzzyTermGroup(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2, pb_FuzzyTerm& op3);
	pb_FuzzyTermGroup(pb_FuzzyTerm& op1, pb_FuzzyTerm& op2, pb_FuzzyTerm& op3, pb_FuzzyTerm& op4);

	double GetDegreeOfMembership()const;
	void ClearDegreeOfMemebership();
	void ORWithDegreeOfMemebership(double val); 
	
	pb_FuzzyTerm* Clone() const { return new pb_FuzzyTermGroup(*this); }

private:

	std::vector<pb_FuzzyTerm*> mTerms;

}