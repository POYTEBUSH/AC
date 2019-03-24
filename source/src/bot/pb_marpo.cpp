#include "cube.h"
#include "pb_marpo.h"

void pb_marpo::AddTarget(pb_target * target, ETaskLevel taskLevel)
{
	//assert that the target has a target position
	//assert(target->)

	switch (taskLevel)
	{
	case TASK_LEVEL_LONGTERM:
		mLongTermTasks.push(target);
		break;
	case TASK_LEVEL_REACTIVE:
		mReactiveTasks.push(target);
		break;
	case TASK_LEVEL_IMMEDIATE:
		mImmediateTasks.push(target);
		break;
	}
}

void pb_marpo::PerformNextTask()
{
	if (mCurrentTarget == nullptr || mCurrentTarget->IsCompleted())
	{
		if (!mImmediateTasks.empty())
		{
			mCurrentTarget = mImmediateTasks.top();
			mImmediateTasks.pop();
		}
		else if (!mReactiveTasks.empty())
		{
			mCurrentTarget = mReactiveTasks.top();
			mReactiveTasks.pop();
		}
		else if (!mLongTermTasks.empty())
		{
			mCurrentTarget = mLongTermTasks.top();
			mLongTermTasks.pop();
		}
		else
		{
			//At this point the long term goal stack is empty, this shouldn't be the case. Re-apply the default.
			mCurrentTarget = mDefaultTarget;
		}
	}
	//We either need to perform the current task or a new one is given.
	mCurrentTarget->PerformTask(mBot);
}
