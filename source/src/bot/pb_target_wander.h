#pragma once
#include "pb_marpo.h"
class pb_target_wander : public pb_target
{

public:
	pb_target_wander(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_wander() {}

	std::vector<pb_target*> CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
};

