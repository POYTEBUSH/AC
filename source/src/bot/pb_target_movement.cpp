#include "cube.h"
#include "pb_target_movement.h"

bool pb_target_movement::CalculateSubTasks(CBot * bot)
{
	//Doesn't currently create sub tasks
	return false;
}

void pb_target_movement::PerformTask(CBot * bot)
{
	//Set the goal waypoint to the one closest to the target vec
	/*bot->SetCurrentWaypoint(bot->GetNearestWaypoint(mTargetVec, 10.f));
	bot->HeadToWaypoint();*/
}

bool pb_target_movement::IsValid(CBot * bot)
{
	//As this is a simple movement task, it is always valid
	return true;
}

bool pb_target_movement::IsCompleted(CBot * bot)
{
	return true;// bot->GetDistance(mTargetVec) <= 2.f;
}

