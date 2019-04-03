#include "cube.h"
#include "pb_target_attack.h"
#include "pb_target_movement.h"

bool pb_target_attack::CalculateSubTasks(CBot * bot)
{
	//If we have the bot but they are unable to be detected, we should make sure they can be 
	if (mTargetBot != nullptr)
	{
		//If we are not in the range specified by the bots skill we need to try and move closer.
		if ((bot->m_pBotSkill->flAlwaysDetectDistance > bot->m_pMyEnt->o.dist(mTargetBot->o)))
		{
			//Create a new sub-task to move the bot towards that location
			pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
			newMovementTask->Set(mTargetBot->o);
			pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(newMovementTask);

			bot->m_iLookForWaypointTime = lastmillis + 250;
			return true;
		}
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
		bot->m_pMyEnt->enemy = mTargetBot;
		bot->ShootEnemy();
		//Stop moving whilst you shoot
		bot->m_pMyEnt->move = 0;
	}
}

bool pb_target_attack::IsValid(CBot * bot)
{
	return (mTargetBot != nullptr && 
		IsInGame(mTargetBot) &&
		bot->DetectEnemy(mTargetBot) &&
		mTargetBot->state == CS_ALIVE);
}

bool pb_target_attack::IsCompleted(CBot * bot)
{
	return mTargetBot->state == CS_DEAD;
}
