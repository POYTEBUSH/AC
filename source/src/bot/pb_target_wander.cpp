#include "cube.h"
#include "pb_target_wander.h"
#include "pb_target_movement.h"
#include "pb_target_hunt.h"
#include "pb_target_reload.h"

#include "pb_FuzzyModule.h"

#include "pb_blackboard.h"
#include "pb_FuzzyPickupCalc.h"

bool pb_target_wander::CalculateSubTasks(CBot * bot)
{
	auto targetvec = bot->GetNearestWaypoint(50.f); 
	
	if (bot->m_pMyEnt->mag[bot->m_pMyEnt->weaponsel->type] == 0)
	{
		pb_target_reload* reloadTask = new pb_target_reload(TASK_LEVEL_REACTIVE);
		//Set the tasks target to the current position
		reloadTask->Set(bot->m_pMyEnt->o);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(reloadTask);

		return true;
	}

	double healthDesire = pb_FuzzyPickupCalc::Instance()->CalculateDesirability(bot->m_pMyEnt, EntityTypes::I_HEALTH);
	double armourDesire = pb_FuzzyPickupCalc::Instance()->CalculateDesirability(bot->m_pMyEnt, EntityTypes::I_ARMOUR);

	if (healthDesire > 50 && healthDesire > armourDesire)
	{

		pb_blackboard_manager::Instance()->AddQuery(pb_blackboard_query(bot->m_pMyEnt, pb_blackboard_query_type::Health), bot->m_pMyEnt->team);
	}
	else if (armourDesire > 50)
	{
		pb_blackboard_manager::Instance()->AddQuery(pb_blackboard_query(bot->m_pMyEnt, pb_blackboard_query_type::Armour), bot->m_pMyEnt->team);
	}
	else
	{
		pb_blackboard_manager::Instance()->AddQuery(pb_blackboard_query(bot->m_pMyEnt, pb_blackboard_query_type::Enemy), bot->m_pMyEnt->team);
	}


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