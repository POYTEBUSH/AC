#include "cube.h"
#include "pb_target_wander.h"
#include "pb_target_movement.h"

bool pb_target_wander::CalculateSubTasks(CBot * bot)
{
	auto targetvec = bot->GetNearestWaypoint(10.f); 
	/*&& (targetvec != bot->m_pCurrentWaypoint)*/
	if (targetvec != nullptr && bot->m_iLookForWaypointTime <= lastmillis)
	{
		//Re add this task back to queue
		//pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(this);

		//Create a new sub-task to move the bot towards that location
		pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
		newMovementTask->Set(targetvec->pNode->v_origin);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(newMovementTask);
		mCompleted = true;
		bot->m_iLookForWaypointTime = lastmillis + 250;
		return true;
	}
	else {
		mCompleted = false;
		return false;
	}

}

void pb_target_wander::PerformTask(CBot * bot)
{
	//This specific task does not have any performance task
}

bool pb_target_wander::IsValid(CBot * bot)
{
	return true;
}