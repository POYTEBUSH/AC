#include "cube.h"
#include "pb_target_attack.h"

bool pb_target_attack::CalculateSubTasks(CBot * bot)
{
	if (mTargetBot != nullptr && bot->DetectEnemy(mTargetBot))
	{

	}
	else
	{
		m
	}
}

void pb_target_attack::PerformTask(CBot * bot)
{

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
	return false;
}
