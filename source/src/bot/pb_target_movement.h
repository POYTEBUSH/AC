#pragma once
#include "pb_marpo.h"

class pb_target_movement : public pb_target
{

public:

	pb_target_movement(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_movement() {};

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);

	vec GetLookAtTarget(CBot* bot) { return vec(mTargetVec.x, mTargetVec.y+4, mTargetVec.z); }
};

