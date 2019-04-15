#pragma once
#include "pb_FuzzySet.h"
#include "pb_FuzzyModule.h"
#include <assert.h>

#include <map>

enum FuzzySetType
{
	RightShoulder,
	LeftShoulder,
	Triangular
};

struct FuzzyRange
{
	double min;
	double max;

	FuzzyRange() { min = 0; max = 0; }

	FuzzyRange(double _min, double _max)
	{
		min = _min;
		max = _max;
	}
};

class pb_FuzzyVariable
{
public:
	pb_FuzzyVariable() {};

	pb_FzSet AddFuzzySet(FuzzySetType type, std::string name, double min, double peak, double max);

	void Fuzzify(double val);
	double DeFuzzify(int numSamples) const;


private:

	std::map<std::string, pb_FuzzySet*> mFuzzySets;

	FuzzyRange mRange;

	void AdjustRangeToBounds(double min, double max);


	pb_FuzzyVariable(const pb_FuzzyVariable&);
	pb_FuzzyVariable& operator=(const pb_FuzzyVariable&);

	//Stop accidental deletion of object
	~pb_FuzzyVariable();
	friend class pb_FuzzyModule;
};

