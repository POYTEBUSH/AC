#pragma once
#include "pb_marpo.h"


class pb_target_rotate : public pb_target
{
public:
	pb_target_rotate(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_rotate() {}

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);

	vec GetLookAtTarget(CBot* bot) { return vec(-1, -1, -1); }
};

