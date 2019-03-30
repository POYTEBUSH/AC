#include "cube.h"
#include "pb_target_movement.h"

std::vector<pb_target*> pb_target_movement::CalculateSubTasks(CBot * bot)
{
	//Check code supplied by base code
	bot->CheckStuck();
	bot->CheckCrouch();
	bot->CheckJump();

	return std::vector<pb_target*>();
}

void pb_target_movement::PerformTask(CBot * bot)
{
	//Set the goal waypoint to the one closest to the target vec
	bot->m_pCurrentGoalWaypoint = bot->GetNearestWaypoint(mTargetVec, 9999.f);

	bot->HeadToGoal();
}

bool pb_target_movement::IsValid(CBot * bot)
{
	//As this is a simple movement task, it is always valid
	return true;
}

