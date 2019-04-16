#pragma once

class pb_FuzzySet
{
public:

	pb_FuzzySet(double RepresentativeVal) : mDegreeOfMembership(.0), mRepresentativeValue(RepresentativeVal) {};
	~pb_FuzzySet() {};

	virtual double CalculateDOM(double val) const = 0;

	void ORWithDOM(double val);

	double GetRepresentativeValue() const { return mRepresentativeValue; }
	double GetDegreeOfMembership() const { return mDegreeOfMembership; }

	void SetDegreeOfMemebership(double val) { mDegreeOfMembership = val; }
	void ResetDegreeOfMemebership() { mDegreeOfMembership = .0; }


private:

	double mDegreeOfMembership;
	double mRepresentativeValue;
};

class pb_FuzzySet_Triange : public pb_FuzzySet
{
public:

	pb_FuzzySet_Triange(double left, double mid, double right) : pb_FuzzySet(mid), mLeftOffset(left), mPeak(mid), mRightOffset(right) {};

	double CalculateDOM(double val) const;

	~pb_FuzzySet_Triange() {}

private:

	double mPeak;
	double mLeftOffset;
	double mRightOffset;
};

class pb_FuzzySet_RightShoulder : public pb_FuzzySet
{
public:

	pb_FuzzySet_RightShoulder(double left, double mid, double right) : pb_FuzzySet(mid), mLeftOffset(left), mPeak(mid), mRightOffset(right) {};

	double CalculateDOM(double val) const;

	~pb_FuzzySet_RightShoulder() {}

private:

	double mPeak;
	double mLeftOffset;
	double mRightOffset;
};

class pb_FuzzySet_LeftShoulder : public pb_FuzzySet
{
public:

	pb_FuzzySet_LeftShoulder(double left, double mid, double right) : pb_FuzzySet(mid), mLeftOffset(left), mPeak(mid), mRightOffset(right) {};

	double CalculateDOM(double val) const;

	~pb_FuzzySet_LeftShoulder() {}

private:

	double mPeak;
	double mLeftOffset;
	double mRightOffset;
};