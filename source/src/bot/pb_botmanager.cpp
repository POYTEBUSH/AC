#include "cube.h"
#include "pb_botmanager.h"

pb_botmanager* pb_botmanager::mInstance;

void pb_botmanager::Init()
{
	pb_FuzzyVariable& targetDistance = mFuzzyModule.CreateFuzzyVariable("TargetDistance");
	auto targetclose = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "TargetClose", 0, 25, 150);
	auto targetnear = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "TargetNear", 25, 50, 300);
	auto targetfar = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "TargetFar", 150, 300, 500);

	pb_FuzzyVariable& targetDistance = mFuzzyModule.CreateFuzzyVariable("Health");
	auto healthlow = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 25, 40);
	auto healthmedium = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "Medium", 25, 50, 75);
	auto healthhigh = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "High", 50, 75, 100);

	pb_FuzzyVariable& targetDistance = mFuzzyModule.CreateFuzzyVariable("Armour");
	auto armourlow = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 10, 20);
	auto armourmedium = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "Medium", 10, 20, 35);
	auto armourhigh = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "High", 30, 50, 100);




	mFuzzyModule.AddRule()
}

void pb_botmanager::Update(std::vector<botent*> bots)
{
}
