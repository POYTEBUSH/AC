#include "cube.h"
#include "pb_marpo.h"

void pb_marpo::AddTask(pb_task * task, ETaskLevel taskLevel)
{
	switch (taskLevel)
	{
	case TASK_LEVEL_LONGTERM:
		mLongTermTasks.push(task);
		break;
	case TASK_LEVEL_REACTIVE:
		mReactiveTasks.push(task);
		break;
	case TASK_LEVEL_IMMEDIATE:
		mImmediateTasks.push(task);
		break;
	}
}

void pb_marpo::PerformNextTask()
{
	if (mCurrentTask == nullptr || mCurrentTask->IsCompleted())
	{
		if (!mImmediateTasks.empty())
		{
			mCurrentTask = mImmediateTasks.top();
			mImmediateTasks.pop();
		}
		else if (!mReactiveTasks.empty())
		{
			mCurrentTask = mReactiveTasks.top();
			mReactiveTasks.pop();
		}
		else if (!mLongTermTasks.empty())
		{
			mCurrentTask = mLongTermTasks.top();
			mLongTermTasks.pop();
		}
		else
		{
			//At this point the long term goal stack is empty, this shouldn't be the case. Re-apply the default.
			mCurrentTask = mDefaultTask;
		}
	}
	//We either need to perform the current task or a new one is given.

	mCurrentTask->PerformTask();
	CBot* bot = new CBot();
	auto test = new pb_task(, TASK_LEVEL_IMMEDIATE);
}
