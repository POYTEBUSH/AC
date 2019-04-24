#include "cube.h"
#include "pb_FuzzyPickupCalc.h"

pb_FuzzyPickupCalc* pb_FuzzyPickupCalc::mInstance;

void pb_FuzzyPickupCalc::Init()
{
	pb_FuzzyVariable& healthScore = mFuzzyModule.CreateFuzzyVariable("Health");
	pb_FzSet healthLow = healthScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 10, 25);
	pb_FzSet healthMedium = healthScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 10, 25, 60);
	pb_FzSet healthFull = healthScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 25, 60, 100);

	pb_FuzzyVariable& armourScore = mFuzzyModule.CreateFuzzyVariable("Armour");
	pb_FzSet armourLow = armourScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 10, 25);
	pb_FzSet armourMedium = armourScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 10, 25, 60);
	pb_FzSet armourFull = armourScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 25, 60, 100);

	pb_FuzzyVariable& desirabilityScore = mFuzzyModule.CreateFuzzyVariable("Desirability");
	pb_FzSet desirabilityMinimal = desirabilityScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 25, 50);
	pb_FzSet desirabilityNeutral = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 25, 50, 75);
	pb_FzSet desirabilityFull = desirabilityScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 50, 75, 100);

	mFuzzyModule.AddRule(pb_FuzzyTermOR(healthLow, armourLow), desirabilityMinimal);
	mFuzzyModule.AddRule(pb_FuzzyTermOR(healthMedium, armourMedium), desirabilityNeutral);
	mFuzzyModule.AddRule(pb_FuzzyTermOR(healthFull, armourFull), desirabilityFull);
}

double pb_FuzzyPickupCalc::CalculateDesirability(playerent* a, EntityTypes type)
{
	//We assume that the other is ignored (full)
	//This is to stop conflict
	switch (type)
	{
	case I_HEALTH:
	{
		mFuzzyModule.Fuzzify("Health", a->health);
		mFuzzyModule.Fuzzify("Armour", 100);
	}
	break;
	case I_ARMOUR:
		mFuzzyModule.Fuzzify("Health", 100);
		mFuzzyModule.Fuzzify("Armour", a->armour);
		break;
	default:
		break;
	}


	double desire = mFuzzyModule.DeFuzzifyMaxAv("Desirability");

	//Desirability is flipped, the desire to collect a pickup is the opposite to the desire to have its gain
	return 100-desire;
}