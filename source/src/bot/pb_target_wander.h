#pragma once
#include "pb_marpo.h"
class pb_target_wander : public pb_target
{

public:
	pb_target_wander(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_wander() {}

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	//Not marked as const due to override
	bool IsCompleted(CBot* bot) { return mCompleted; }

private:
	bool mCompleted;
};

