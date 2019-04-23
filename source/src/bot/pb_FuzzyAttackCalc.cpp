#include "cube.h"
#include "pb_FuzzyAttackCalc.h"

pb_FuzzyAttackCalc* pb_FuzzyAttackCalc::mInstance;

void pb_FuzzyAttackCalc::Init()
{
	pb_FuzzyVariable& desirabilityScore = mFuzzyModule.CreateFuzzyVariable("Desirability");
	pb_FzSet desirabilityMinimal = desirabilityScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 25, 50);
	pb_FzSet desirabilityNeutral = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 25, 50, 75);
	pb_FzSet desirabilityFull = desirabilityScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 50, 75, 100);

	for (size_t i = 0; i < MAX_WEAPONS; i++)
	{
		std::stringstream ss;
		ss << "Weapon_";
		ss << i;

		pb_FuzzyVariable& ammoCount = mFuzzyModule.CreateFuzzyVariable(ss.str());
		pb_FzSet ammoLow = ammoCount.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, WeaponInfoTable[i].sMinDesiredAmmo / 2, WeaponInfoTable[i].sMinDesiredAmmo);
		pb_FzSet ammoSufficient = ammoCount.AddFuzzySet(FuzzySetType::RightShoulder, "Sufficient", WeaponInfoTable[i].sMinDesiredAmmo / 2, WeaponInfoTable[i].sMinDesiredAmmo, ammostats[i].max);

		pb_FuzzyVariable& enemyDistance = mFuzzyModule.CreateFuzzyVariable(ss.str() + "EnemyDistance");
		pb_FzSet enemyClose = enemyDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "EnemyClose", 0, WeaponInfoTable[i].flMinFireDistance, WeaponInfoTable[i].flMinDesiredDistance);
		pb_FzSet enemyNear = enemyDistance.AddFuzzySet(FuzzySetType::Triangular, "EnemyNear", WeaponInfoTable[i].flMinDesiredDistance, WeaponInfoTable[i].flMaxDesiredDistance, WeaponInfoTable[i].flMaxFireDistance);
		pb_FzSet enemyFar = enemyDistance.AddFuzzySet(FuzzySetType::RightShoulder, "EnemyFar", WeaponInfoTable[i].flMaxDesiredDistance, WeaponInfoTable[i].flMaxFireDistance, 1e4);

		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, ammoLow), desirabilityMinimal);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, ammoLow), desirabilityMinimal);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, ammoLow), desirabilityMinimal);

		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, ammoSufficient), desirabilityNeutral);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, ammoSufficient), desirabilityFull);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, ammoSufficient), desirabilityMinimal);
	}
}

double pb_FuzzyAttackCalc::CalculateDesirability(playerent* a, playerent* target)
{
	std::stringstream ss;
	ss << "Weapon_";
	ss << a->weaponsel->type;

	mFuzzyModule.Fuzzify(ss.str() + "EnemyDistance", a->o.dist(target->o));
	mFuzzyModule.Fuzzify(ss.str(), a->mag[a->weaponsel->type]);

	double desire = mFuzzyModule.DeFuzzifyMaxAv("Desirability");

	return desire;
}

double pb_FuzzyAttackCalc::CalculateDesirability(playerent* a, vec target)
{
	std::stringstream ss;
	ss << "Weapon_";
	ss << a->weaponsel->type;

	mFuzzyModule.Fuzzify(ss.str() + "EnemyDistance", a->o.dist(target));
	mFuzzyModule.Fuzzify(ss.str(), a->mag[a->weaponsel->type]);

	double desire = mFuzzyModule.DeFuzzifyMaxAv("Desirability");

	return desire;
}
