#include "cube.h"
#include "pb_target_movement.h"

#include "pb_target_rotate.h"

bool pb_target_movement::CalculateSubTasks(CBot * bot)
{
	//Doesn't currently create sub tasks
	if (bot->m_iStuckCheckDelay + (bot->CheckCrouch() ? 2000 : 0) >= lastmillis)
		return false;

	bool IsStuck = false;
	vec CurPos = bot->m_pMyEnt->o, PrevPos = bot->m_vPrevOrigin;
	CurPos.z = PrevPos.z = 0;
	// Did the bot hardly move the last frame?
	if (GetDistance(CurPos, PrevPos) <= 0.1f)
	{
		if (bot->m_bStuck)
		{
			if (bot->m_iStuckTime < lastmillis)
				IsStuck = true;
		}
		else
		{
			bot->m_bStuck = true;
			bot->m_iStuckTime = lastmillis + 1000;
		}
	}
	else
	{
		bot->m_bStuck = false;
		bot->m_iStuckTime = 0;
	}

	if (IsStuck)
	{
		pb_target_rotate* rotationTask = new pb_target_rotate(TASK_LEVEL_REACTIVE);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(rotationTask);
		return true;
	}

	return false;
}

void pb_target_movement::PerformTask(CBot * bot)
{	//Set the goal waypoint to the one closest to the target vec
	//Start with the bot moving
	if (mTargetVec != vec(-1,-1,-1))
	{
		bot->SetCurrentWaypoint(bot->GetNearestWaypoint(mTargetVec, 10.f));
		bot->HeadToWaypoint();
	}
}

bool pb_target_movement::IsValid(CBot * bot)
{
	//As this is a simple movement task, it is always valid
	return true;// bot->IsReachable(mTargetVec);
}

bool pb_target_movement::IsCompleted(CBot * bot)
{
	//Utilised for hunt task
	if(mTargetEntity != nullptr)
	{
		return bot->GetDistance(mTargetVec) <= 10.f && mTargetEntity->spawned == false;
	}
	else if (bot->GetDistance(mTargetVec) <= 5.f)
	{
		bot->ResetWaypointVars();
		return true;
	}
	return false;
}

