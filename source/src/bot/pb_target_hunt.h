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

	void SetTargetType(HUNT_TARGET_TYPE type) { mHuntType = type; }

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);

private:

	HUNT_TARGET_TYPE mHuntType;
	bool mTaskFound;
};

