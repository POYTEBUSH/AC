#include "cube.h"
#include "pb_botmanager.h"

//MARPO Tasks
#include "pb_target_attack.h"

pb_botmanager* pb_botmanager::mInstance;

void pb_botmanager::Init()
{
	pb_FuzzyVariable& targetDistance = mFuzzyModule.CreateFuzzyVariable("TargetDistance");
	pb_FzSet targetClose = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "TargetClose", 0, 5, 15);
	pb_FzSet targetNear = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "TargetNear", 10, 50, 75);
	pb_FzSet targetFar = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "TargetFar", 60, 125, 500);

	pb_FuzzyVariable& enemyDistance = mFuzzyModule.CreateFuzzyVariable("EnemyDistance");
	pb_FzSet enemyClose = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "EnemyClose", 0, 5, 15);
	pb_FzSet enemyNear = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "EnemyNear", 10, 50, 75);
	pb_FzSet enemyFar = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "EnemyFar", 60, 125, 500);

	pb_FuzzyVariable& botHealth = mFuzzyModule.CreateFuzzyVariable("Health");
	pb_FzSet healthLow = botHealth.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 25, 40);
	pb_FzSet healthMedium = botHealth.AddFuzzySet(FuzzySetType::Triangular, "Medium", 25, 50, 75);
	pb_FzSet healthHigh = botHealth.AddFuzzySet(FuzzySetType::RightShoulder, "High", 50, 75, 100);

	pb_FuzzyVariable& botArmour = mFuzzyModule.CreateFuzzyVariable("Armour");
	pb_FzSet armourLow = botArmour.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 10, 20);
	pb_FzSet armourMedium = botArmour.AddFuzzySet(FuzzySetType::Triangular, "Medium", 10, 20, 35);
	pb_FzSet armourHigh = botArmour.AddFuzzySet(FuzzySetType::RightShoulder, "High", 30, 50, 100);

	pb_FuzzyVariable& desirabilityScore = mFuzzyModule.CreateFuzzyVariable("Desirability");
	pb_FzSet desirabilityMinimal = desirabilityScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 25, 30);
	pb_FzSet desirabilityLow = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "LowDesirability", 30, 40, 50);
	pb_FzSet desirabilityNeutral = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 40, 50, 60);
	pb_FzSet desirabilityHigh = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "HighDesirability", 50, 60, 70);
	pb_FzSet desirabilityFull = desirabilityScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 70, 75, 100);

	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, healthLow, armourLow), desirabilityFull);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, healthLow), desirabilityFull);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, armourLow), desirabilityFull);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, healthMedium), desirabilityFull);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, healthHigh), desirabilityFull);

	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, healthLow, armourLow), desirabilityLow);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, healthLow), desirabilityNeutral);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, armourLow), desirabilityNeutral);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, healthMedium), desirabilityHigh);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, healthHigh), desirabilityFull);

	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, healthLow, armourLow), desirabilityMinimal);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, healthLow), desirabilityLow);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, armourLow), desirabilityLow);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, healthMedium), desirabilityNeutral);
	mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, healthHigh), desirabilityNeutral);

	for (size_t i = 0; i < MAX_WEAPONS; i++)
	{
		pb_FuzzyVariable& ammoCount = mFuzzyModule.CreateFuzzyVariable("Weapon_" + i);
		pb_FzSet ammoLow = ammoCount.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, WeaponInfoTable[i].sMinDesiredAmmo / 2, WeaponInfoTable[i].sMinDesiredAmmo);
		pb_FzSet ammoSufficient = ammoCount.AddFuzzySet(FuzzySetType::RightShoulder, "Sufficient", WeaponInfoTable[i].sMinDesiredAmmo / 2, WeaponInfoTable[i].sMinDesiredAmmo, ammostats[i].max);

		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, ammoLow), desirabilityMinimal);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, ammoLow), desirabilityMinimal);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, ammoLow), desirabilityMinimal);

		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyClose, ammoSufficient), desirabilityFull);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyNear, ammoSufficient), desirabilityNeutral);
		mFuzzyModule.AddRule(pb_FuzzyTermGroup(enemyFar, ammoSufficient), desirabilityLow);
	}
}

void pb_botmanager::Update(vector<botent*> bots)
{
	for (size_t i = 0; i < bots.length(); i++)
	{
		auto thisBotEnt = bots[i];
		auto thisBot = bots[i]->pBot;


		if (!bots[i]) continue;
		if (bots[i]->pBot)
		{
			auto botMarpoI = pb_marpomanager::Instance().GetBotAttachment(thisBotEnt);
			
			if (thisBotEnt->state == CS_ALIVE) {
				mFuzzyModule.Fuzzify("Health", max(0, thisBotEnt->health));
				mFuzzyModule.Fuzzify("Armour", max(0, thisBotEnt->armour));

				//Check the desirability of the current task
				auto currentTarget = botMarpoI->GetCurrentTarget();
				double currentBestScore = 0.0;
				if (currentTarget != nullptr)
				{
					double targetDist = thisBot->GetDistance(currentTarget->GetTargetPos());
					mFuzzyModule.Fuzzify("TargetDistance", targetDist);
					currentBestScore = mFuzzyModule.DeFuzzifyMaxAv("Desirability");
				}


				playerent* target = nullptr;

				double targetBestScore = 0.0;

				int weapon = thisBotEnt->weaponsel->type;
				//Don't bother looking to attack a bot if you have no ammo for it
				for (size_t j = 0; j < bots.length(); j++)
				{
					botent* enemyBot = bots[j];
					if (enemyBot->state == CS_ALIVE) {
						if (thisBot->IsInFOV(enemyBot) && (enemyBot->team != thisBotEnt->team || m_arena))
						{
							mFuzzyModule.Fuzzify("TargetDistance", thisBot->GetDistance(enemyBot->o));
							mFuzzyModule.Fuzzify("Weapon_"+ weapon, thisBotEnt->mag[weapon]);

							double desireToAttack = mFuzzyModule.DeFuzzifyMaxAv("Desirability");
							if (desireToAttack > targetBestScore)
							{
								targetBestScore = desireToAttack;
								target = enemyBot;
							}
						}
					}
				}
				if (player1->state == CS_ALIVE) {
					if (thisBot->IsInFOV(player1) && (player1->team != thisBotEnt->team || m_arena))
					{
						mFuzzyModule.Fuzzify("TargetDistance", thisBot->GetDistance(player1->o));
						mFuzzyModule.Fuzzify("Weapon_" + weapon, thisBotEnt->mag[weapon]);

						double desireToAttack = mFuzzyModule.DeFuzzifyMaxAv("Desirability");
						if (desireToAttack > targetBestScore)
						{
							targetBestScore = desireToAttack;
							target = player1;
						}
					}
				}

				//Check if we have a new target and the new target has a higher desirability,
				//The new target must either be more desirable, no current target or a higher risk
				if (target != nullptr && 
					(targetBestScore > currentBestScore 
						|| currentTarget == nullptr 
						|| (currentTarget->GetTaskLevel() < TASK_LEVEL_REACTIVE)))
				{
					auto attackTask = new pb_target_attack(TASK_LEVEL_REACTIVE);
					attackTask->Set(target);
					botMarpoI->AddTarget(attackTask);
				}
			}
			bots[i]->pBot->Think();
		}
	}	
}