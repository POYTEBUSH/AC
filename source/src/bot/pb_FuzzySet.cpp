#include "cube.h"
#include "pb_FuzzySet.h"


void pb_FuzzySet::ORWithDOM(double val)
{
	if (val > mDegreeOfMembership) mDegreeOfMembership = val;
}


double pb_FuzzySet_Triange::CalculateDOM(double val) const
{
	if (mRightOffset == 0.0 && mPeak == val ||
		mLeftOffset == 0.0 && mPeak == val)
		return 1.0;

	if ((val <= mPeak) && (val >= (mPeak - mLeftOffset)))
	{
		double grad = 1.0 / mLeftOffset;
		return grad * (val - (mPeak - mLeftOffset));
	}
	else if ((val > mPeak) && (val < (mPeak + mRightOffset)))
	{
		double grad = 1.0 / -mRightOffset;
		return grad * (val - mPeak) + 1.0;
	}
	return 0.0;
}

double pb_FuzzySet_RightShoulder::CalculateDOM(double val) const
{
	if (mLeftOffset == 0.0 && mPeak == val)
		return 1.0;

	if ((val <= mPeak) && (val > (mPeak - mLeftOffset)))
	{
		double grad = 1.0 / mLeftOffset;
		return grad * (val - (mPeak - mLeftOffset));
	}
	else if (val > mPeak)
	{
		return 1.0;
	}
	return 0.0;
}

double pb_FuzzySet_LeftShoulder::CalculateDOM(double val) const
{
	if (mRightOffset == 0.0 && mPeak == val)
		return 1.0;

	if ((val >= mPeak) && (val <= (mPeak + mRightOffset)))
	{
		double grad = 1.0 / -mRightOffset;
		return grad * (val - mPeak) + 1.0;
	}
	else if (val < mPeak)
	{
		return 1.0;
	}
	return 0.0;
}
