#include "cube.h"
#include "pb_target_wander.h"
#include "pb_target_movement.h"

std::vector<pb_target*> pb_target_wander::CalculateSubTasks(CBot * bot)
{
	//Find what the bot is looking at currently

	auto targetvec = bot->GetNearestWaypoint(50.f);
	//can we see the node
	if (!bot->IsInFOV(targetvec->pNode->v_origin))
	{
		bot->AimToVec(targetvec->pNode->v_origin);
	}

	pb_target_movement* movementSubTask = new pb_target_movement(mTaskLevel);
	movementSubTask->Set(targetvec->pNode->v_origin);

	pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(movementSubTask, mTaskLevel);

	//At this point we have supplied a sub-task, this tasks job is complete.
	mIsCompleted = true;
	return std::vector<pb_target*>{ movementSubTask };
}

void pb_target_wander::PerformTask(CBot * bot)
{
	//This specific task does not have any performance task
}

bool pb_target_wander::IsValid(CBot * bot)
{
	return true;
}