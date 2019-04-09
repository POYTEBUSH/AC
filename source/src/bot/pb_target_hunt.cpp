#include "cube.h"
#include "pb_target_hunt.h"

bool pb_target_hunt::CalculateSubTasks(CBot * bot)
{
	switch (mHuntType)
	{
	case HUNT_TARGET_ENEMY:
		return false;
	case HUNT_TARGET_AMMO:
	{

	}
	return true;
	case HUNT_TARGET_HEALTH:

		return true;
	default:
		return false;
	}
	return false;
}

void pb_target_hunt::PerformTask(CBot * bot)
{
}

bool pb_target_hunt::IsValid(CBot * bot)
{
	return false;
}

bool pb_target_hunt::IsCompleted(CBot * bot)
{
	return false;
}
