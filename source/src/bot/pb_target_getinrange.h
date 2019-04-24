#pragma once
#include "pb_marpo.h"

//Distance to achieve before task is deemed completed
class pb_target_getinrange : public pb_target
{

public:

	pb_target_getinrange(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_getinrange() {};

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);

	void SetRange(double range) { mRequiredRange = range; }

	vec GetLookAtTarget(CBot* bot) { return vec(-1, -1, -1); }// return vec(mTargetVec.x, mTargetVec.y + 4, mTargetVec.z); }

private:

	double mRequiredRange = 1e2;
};

