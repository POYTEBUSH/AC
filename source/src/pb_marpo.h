#pragma once
#include "pb_target.h"

#include <stack>

class pb_marpo
{
public:

	void AddTarget(pb_target* target, ETaskLevel taskLevel);
	void SetDefaultTarget(pb_target* target) { mDefaultTarget = target; };

	void PerformNextTask();

private:

	std::stack<pb_target*> mLongTermTasks;
	std::stack<pb_target*> mReactiveTasks;
	std::stack<pb_target*> mImmediateTasks;

	//The goal which will always be performed
	pb_target* mDefaultTarget;

	pb_target* mCurrentTarget;
};
