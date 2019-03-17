#pragma once
#include "pb_target.h"

#include <stack>

enum ETaskLevel {
	TASK_LEVEL_LONGTERM,
	TASK_LEVEL_REACTIVE,
	TASK_LEVEL_IMMEDIATE
};

class pb_marpo
{
public:

	void AddTask(pb_task* task, ETaskLevel taskLevel);
	void SetDefaultTask(pb_task* task) { mDefaultTask = task; };

	void PerformNextTask();

private:

	std::stack<pb_task*> mLongTermTasks;
	std::stack<pb_task*> mReactiveTasks;
	std::stack<pb_task*> mImmediateTasks;

	//The goal which will always be performed
	pb_task* mDefaultTask;

	pb_task* mCurrentTask;
};
