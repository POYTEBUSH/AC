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

private:

	entity* FindEntity(CBot * bot, EntityTypes type);

	EntityTypes mHuntType;
	bool mTaskFound;
};

