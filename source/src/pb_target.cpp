#include "cube.h"
#include "pb_target.h"

pb_target::pb_target(ETaskLevel taskLevel)
{
	mTaskLevel = taskLevel;
	mTargetCompletionRange = 0.f;
	mTargetType = ETargetType::TARGET_TYPE_NONE;
	mTargetEntity = nullptr;
	mTargetBot = nullptr;
}

pb_target::~pb_target() {}

void pb_target::Set(const vec & position)
{
	mTargetVec = position;
	mTargetType = ETargetType::TARGET_TYPE_NONE;	
}

void pb_target::Set(const entity * entity)
{
	mTargetEntity = entity;

	//entity does not have its own vec, need to convert from raw coords
	mTargetVec = vec(entity->x, entity->y, entity->z);

	mTargetType = ETargetType::TARGET_TYPE_ENTITY;
}

void pb_target::Set(const playerent * entity)
{
	mTargetBot = entity;

	//entity does not have its own vec, need to convert from raw coords
	mTargetVec = entity->o;

	mTargetType = ETargetType::TARGET_TYPE_BOT;
}

std::vector<pb_target*> pb_target_movement::CalculateSubTasks(CBot* bot)
{
	//TODO Do I need to jump or duck
	return std::vector<pb_target*>();
}

void pb_target_movement::PerformTask(CBot* bot)
{
	//If we don't have a target pos something is wrong
	assert(mTargetVec != nullptr);
	
	if (bot->GetDistance(mTargetVec)) 
	{
		mIsCompleted;
		return;
	}

	bot->m_pCurrentGoalWaypoint = bot->GetNearestWaypoint(mTargetVec, 9999.f);
	bot->HeadToGoal();
	runtime++;
}
