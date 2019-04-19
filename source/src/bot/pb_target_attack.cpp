#include "cube.h"
#include "pb_target_attack.h"
#include "pb_target_movement.h"
#include "pb_target_reload.h"

#include <iostream>

bool pb_target_attack::CalculateSubTasks(CBot * bot)
{
	//If we have the bot but they are unable to be detected, we should make sure they can be 
	if (bot->m_pMyEnt->mag[bot->m_pMyEnt->weaponsel->type] == 0)
	{
		//Re add this task back to queue
		//\pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(this);

		pb_target_reload* reloadTask = new pb_target_reload(mTaskLevel);
		//Set the tasks target to the current position
		reloadTask->Set(bot->m_pMyEnt->o);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(reloadTask);

		return true;
	}
	if (mTargetBot != nullptr)
	{
		////If we are not in the range specified by the bots skill we need to try and move closer.
		//if ((bot->m_pBotSkill->flAlwaysDetectDistance < bot->m_pMyEnt->o.dist(mTargetBot->o)))
		//{
		//	//Create a new sub-task to move the bot towards that location
		//	pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
		//	newMovementTask->Set(mTargetBot->o);
		//	pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(newMovementTask);

		//	bot->m_iLookForWaypointTime = lastmillis + 250;
		//	return true;
		//}
		if (!bot->IsInFOV(mTargetBot))
		{
			//For now we are just going to force the bot to look, maybe later add a subtask
			bot->AimToVec(mTargetBot->head);
		}
	}
	return false;
}

void pb_target_attack::PerformTask(CBot * bot)
{
	if (mTargetBot) {
		//Stop the bot moving when they are targeting an enemy, if they get too far away it will target them instead.
		bot->m_pMyEnt->move = 0;

		bot->m_pMyEnt->enemy = mTargetBot;
		bot->AimToVec(mTargetBot->head);
		bot->ShootEnemy();

		bot->ResetWaypointVars();
		bot->m_eCurrentBotState = STATE_ENEMY;
	}
}

bool pb_target_attack::IsValid(CBot * bot)
{
	bool validenemy = (mTargetBot != nullptr &&
		IsInGame(mTargetBot) &&
		bot->DetectEnemy(mTargetBot) &&
		mTargetBot->state == CS_ALIVE);

	float flDist = GetDistance(bot->m_pMyEnt->o, mTargetBot->o);

	// Check if bot is in fire range
	bool inRange = (flDist > WeaponInfoTable[bot->m_pMyEnt->gunselect].flMinFireDistance) ||
		(flDist < WeaponInfoTable[bot->m_pMyEnt->gunselect].flMaxFireDistance);

	return validenemy && inRange;
}

bool pb_target_attack::IsCompleted(CBot * bot)
{
	return mTargetBot->state == CS_DEAD;
}

vec pb_target_attack::GetLookAtTarget(CBot * bot)
{
	if (mTargetBot->head == vec(-1, -1, -1))
	{
		return vec(mTargetBot->o.x, mTargetBot->o.y, mTargetBot->o.z);
	}
	return mTargetBot->head;
}
