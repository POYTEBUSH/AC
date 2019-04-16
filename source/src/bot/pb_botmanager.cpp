#include "cube.h"
#include "pb_botmanager.h"

pb_botmanager* pb_botmanager::mInstance;

void pb_botmanager::Init()
{
	pb_FuzzyVariable& targetDistance = mFuzzyModule.CreateFuzzyVariable("TargetDistance");
	pb_FzSet targetClose = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "TargetClose", 0, 25, 150);
	pb_FzSet targetNear = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "TargetNear", 25, 50, 300);
	pb_FzSet targetFar = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "TargetFar", 150, 300, 500);

	pb_FuzzyVariable& botHealth = mFuzzyModule.CreateFuzzyVariable("Health");
	pb_FzSet healthLow = botHealth.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 25, 40);
	pb_FzSet healthMedium = botHealth.AddFuzzySet(FuzzySetType::Triangular, "Medium", 25, 50, 75);
	pb_FzSet healthHigh = botHealth.AddFuzzySet(FuzzySetType::RightShoulder, "High", 50, 75, 100);

	pb_FuzzyVariable& botArmour = mFuzzyModule.CreateFuzzyVariable("Armour");
	pb_FzSet armourLow = botArmour.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 10, 20);
	pb_FzSet armourMedium = botArmour.AddFuzzySet(FuzzySetType::Triangular, "Medium", 10, 20, 35);
	pb_FzSet armourHigh = botArmour.AddFuzzySet(FuzzySetType::RightShoulder, "High", 30, 50, 100);

	pb_FuzzyVariable& desirabilityScore = mFuzzyModule.CreateFuzzyVariable("Desirability");
	pb_FzSet desirabilityLow = desirabilityScore.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 30, 50);
	pb_FzSet desirabilityNeutral = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "Neutral", 30, 50, 70);
	pb_FzSet desirabilityHigh = desirabilityScore.AddFuzzySet(FuzzySetType::RightShoulder, "High", 50, 70, 100);

	mFuzzyModule.AddRule(pb_FuzzyTermGroup(targetClose, healthLow), desirabilityLow);




}

void pb_botmanager::Update(std::vector<botent*> bots)
{
}
