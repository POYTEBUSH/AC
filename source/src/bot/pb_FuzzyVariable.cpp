#include "cube.h"
#include "pb_FuzzyVariable.h"

pb_FzSet pb_FuzzyVariable::AddFuzzySet(FuzzySetType type, std::string name, double min, double peak, double max)
{
	switch (type)
	{
	case RightShoulder:
		mFuzzySets[name] = new pb_FuzzySet_RightShoulder(min, peak, max);
	break;
	case LeftShoulder:
		mFuzzySets[name] = new pb_FuzzySet_LeftShoulder(min, peak, max);
		break;
	case Triangular:
		mFuzzySets[name] = new pb_FuzzySet_Triange(min, peak, max);
		break;
	default:
		break;
	}

	AdjustRangeToBounds(min, max);
	return pb_FzSet(*mFuzzySets[name]);
}

void pb_FuzzyVariable::Fuzzify(double val)
{
	//make sure the value is within the bounds of this variable
	assert((val >= mRange.min) && (val <= mRange.max));

	for (auto fs : mFuzzySets)
	{
		fs.second->SetDegreeOfMemebership(fs.second->CalculateDOM(val));
	}
}

double pb_FuzzyVariable::DeFuzzifyCentroid(int numSamples) const
{
	double StepSize = (mRange.max - mRange.min) / (double)numSamples;

	double total = 0.0;
	double sum = 0.0;

	//For each step to make, calculate the min value between the current degree of memebership and the value at that step
	for (int i = 1; i <= numSamples; ++i)
	{
		for (auto fs : mFuzzySets)
		{
			double minVal = min(fs.second->CalculateDOM(mRange.min + i * StepSize), fs.second->GetDegreeOfMembership());
			total += minVal;

			sum += (mRange.min + i * StepSize) * minVal;

		}
	}

	return total == 0.0 ? 0.0 : sum / total;
}

double pb_FuzzyVariable::DeFuzzifyMaxAv() const
{
	double bottom = 0.0;
	double top = 0.0;

	for (auto fs : mFuzzySets)
	{
		auto fsDOM = fs.second->GetDegreeOfMembership();
		bottom += fsDOM;
		top += fs.second->GetRepresentativeValue() * fsDOM;
	}

	return bottom == 0.0 ? 0.0 : top / bottom;
}


void pb_FuzzyVariable::AdjustRangeToBounds(double min, double max)
{
	if (min < mRange.min) mRange.min = min;
	if (max > mRange.max) mRange.max = max;
}

pb_FuzzyVariable::~pb_FuzzyVariable()
{
	for (auto fs : mFuzzySets)
	{
		delete fs.second;
	}
}
