#pragma once
#include "pb_marpo.h"

enum HUNT_TARGET_TYPE
{
	HUNT_TARGET_ENEMY,
	HUNT_TARGET_AMMO,
	HUNT_TARGET_HEALTH
};

class pb_target_hunt : public pb_target
{
public:
	pb_target_hunt(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_hunt() {};

	void SetTargetType(EntityTypes type) { mHuntType = type; }

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);

	vec GetLookAtTarget(CBot* bot) { if (mTargetEntity) return vec(mTargetEntity->x, mTargetEntity->y, mTargetEntity->z); return vec(-1,-1,-1); }

private:

	entity* FindEntity(CBot * bot, EntityTypes type);

	entity* mFoundEntity;

	EntityTypes mHuntType;
	bool mTaskFound = false;

	bool mRotationPerformed = false;
};

