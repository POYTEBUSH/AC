#include "cube.h"
#include "pb_target_wander.h"
#include "pb_target_movement.h"
#include "pb_target_hunt.h"
#include "pb_target_reload.h"

#include "pb_FuzzyModule.h"

#include "pb_blackboard.h"

bool pb_target_wander::CalculateSubTasks(CBot * bot)
{
	auto targetvec = bot->GetNearestWaypoint(50.f); 


	//NOTE THIS FUZZY SYSTEM IS THE DESIRABILITY OF FINIDING SOME ITEMS FOR THIS TYPE
	//NOT THE DESIRABILITY TO BE LOW HEALTH OR ARMOUR
	//pb_FuzzyModule mFuzzyModule;
	//pb_FuzzyModule mFuzzyModule2;

	//pb_FuzzyVariable& botHealth = mFuzzyModule.CreateFuzzyVariable("Health");
	//pb_FzSet healthLow = botHealth.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 25, 50);
	//pb_FzSet healthMedium = botHealth.AddFuzzySet(FuzzySetType::Triangular, "Medium", 45, 50, 75);
	//pb_FzSet healthHigh = botHealth.AddFuzzySet(FuzzySetType::RightShoulder, "High", 70, 75, 100);

	//pb_FuzzyVariable& botArmour = mFuzzyModule2.CreateFuzzyVariable("Armour");
	//pb_FzSet armourLow = botArmour.AddFuzzySet(FuzzySetType::LeftShoulder, "Low", 0, 10, 20);
	//pb_FzSet armourMedium = botArmour.AddFuzzySet(FuzzySetType::Triangular, "Medium", 15, 20, 35);
	//pb_FzSet armourHigh = botArmour.AddFuzzySet(FuzzySetType::RightShoulder, "High", 30, 50, 100);

	//pb_FuzzyVariable& desirabilityScore = mFuzzyModule.CreateFuzzyVariable("Desirability");
	//pb_FzSet desirabilityMinimal = desirabilityScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 25, 30);
	//pb_FzSet desirabilityNeutral = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 25, 50, 65);
	//pb_FzSet desirabilityFull = desirabilityScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 60, 75, 100);

	//pb_FuzzyVariable& desirabilityScore2 = mFuzzyModule2.CreateFuzzyVariable("Desirability");
	//pb_FzSet desirabilityMinimal2 = desirabilityScore2.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 25, 40);
	//pb_FzSet desirabilityNeutral2 = desirabilityScore2.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 35, 50, 65);
	//pb_FzSet desirabilityFull2 = desirabilityScore2.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 60, 75, 100);

	//mFuzzyModule.AddRule(healthLow, desirabilityFull);
	//mFuzzyModule.AddRule(healthMedium, desirabilityNeutral);
	//mFuzzyModule.AddRule(healthHigh, desirabilityMinimal);

	//mFuzzyModule2.AddRule(armourLow, desirabilityFull2);
	//mFuzzyModule2.AddRule(armourMedium, desirabilityNeutral2);
	//mFuzzyModule2.AddRule(armourHigh, desirabilityMinimal2);

	//mFuzzyModule.Fuzzify("Health", max(0, bot->m_pMyEnt->health));
	//auto healthHuntDesire = mFuzzyModule.DeFuzzifyMaxAv("Desirability");

	//mFuzzyModule2.Fuzzify("Armour", max(0, bot->m_pMyEnt->armour));
	//auto armourHuntDesire = mFuzzyModule2.DeFuzzifyMaxAv("Desirability");

	//If we have the bot but they are unable to be detected, we should make sure they can be 
	if (bot->m_pMyEnt->mag[bot->m_pMyEnt->weaponsel->type] == 0)
	{
		//Re add this task back to queue
		//\pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(this);

		pb_target_reload* reloadTask = new pb_target_reload(TASK_LEVEL_REACTIVE);
		//Set the tasks target to the current position
		reloadTask->Set(bot->m_pMyEnt->o);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(reloadTask);

		return true;
	}
	pb_blackboard_manager::Instance()->AddQuery(pb_blackboard_query(bot->m_pMyEnt, pb_blackboard_query_type::Enemy), bot->m_pMyEnt->team);


	if (targetvec != nullptr && (targetvec != bot->m_pCurrentWaypoint) && bot->m_iLookForWaypointTime <= lastmillis)
	{
		//Look for any entities to remember
		auto botMarpo = pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt);
		auto locationMem = botMarpo->GetPickupLocationMemory();
		loopv(ents)
		{
			auto &e = ents[i];
			//Check if the entity is one that is required and if it's in the bots view
			if (bot->IsInFOV(vec(e.x, e.y, e.z)) && bot->IsReachable(vec(e.x, e.y, e.z)))
				locationMem->Add(&e, true);
		}

		//Create a new sub-task to move the bot towards that location
		pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
		newMovementTask->Set(targetvec->pNode->v_origin);
		botMarpo->AddTarget(newMovementTask);
		mCompleted = true;
		bot->m_iLookForWaypointTime = lastmillis + 250;
		return true;
	}
	//if (healthHuntDesire > 50 && healthHuntDesire > armourHuntDesire)
	//{
	//	pb_target_hunt* huntTask = new pb_target_hunt(mTaskLevel);
	//	//Set the tasks target to the current position
	//	huntTask->SetTargetType(EntityTypes::I_HEALTH);
	//	pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(huntTask);
	//	mCompleted = true;
	//	return true;
	//}
	//else if (armourHuntDesire > 50 && armourHuntDesire > healthHuntDesire)
	//{
	//	pb_target_hunt* huntTask = new pb_target_hunt(mTaskLevel);
	//	//Set the tasks target to the current position
	//	huntTask->SetTargetType(EntityTypes::I_ARMOUR);
	//	pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(huntTask);
	//	mCompleted = true;
	//	return true;
	//}
	mCompleted = false;
	return false;
}

void pb_target_wander::PerformTask(CBot * bot)
{
	bot->m_pMyEnt->move = 1;
	//This specific task does not have any performance task
}

bool pb_target_wander::IsValid(CBot * bot)
{
	return true;
}