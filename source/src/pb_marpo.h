#pragma once
#include "pb_target.h"

#include <stack>

class pb_marpo
{
public:


private:

	static std::stack<pb_task*> mLongTermTasks;
	static std::stack<pb_task*> mReactiveTasks;
	static std::stack<pb_task*> mImmediateTasks;

	//The goal which will always be performed when not 
	static pb_task* mMainTasks;
};
