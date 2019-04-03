#pragma once
#include "pb_marpo.h"

class pb_target_attack : public pb_target
{
public:
	pb_target_attack(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_attack() {};

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);
};

