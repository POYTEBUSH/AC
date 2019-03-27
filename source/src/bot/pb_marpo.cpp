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
	if (mCurrentTarget == nullptr || mCurrentTarget->IsCompleted() || mCurrentTarget->IsValid(mBot->pBot))
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
	//Check if there are any subtasks to perform
	auto subtasks = mCurrentTarget->CalculateSubTasks(mBot->pBot);

	if (!subtasks.empty()) {
		for (auto t : subtasks)
		{
			AddTarget(t, mCurrentTarget->GetTaskLevel());
		}
		//Set the current to nullptr so when it loops around again we use the new task
		mCurrentTarget = nullptr;
		return;
	}

	//We either need to perform the current task or a new one is given.
	mCurrentTarget->PerformTask(mBot->pBot);
}
