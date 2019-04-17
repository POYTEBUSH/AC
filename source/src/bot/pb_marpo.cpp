#include "cube.h"
#include "pb_marpo.h"

pb_target::pb_target(ETaskLevel taskLevel)
{
	mTaskLevel = taskLevel;
	mTargetCompletionRange = 0.f;
	mTargetType = ETargetType::TARGET_TYPE_NONE;
	mTargetEntity = nullptr;
	mTargetBot = nullptr;
}

pb_target::~pb_target() {}

void pb_target::Set(const vec & position)
{
	mTargetVec = position;
	mTargetType = ETargetType::TARGET_TYPE_NONE;
}

void pb_target::Set(entity * entity)
{
	mTargetEntity = entity;

	//entity does not have its own vec, need to convert from raw coords
	mTargetVec = vec(entity->x, entity->y, entity->z);

	mTargetType = ETargetType::TARGET_TYPE_ENTITY;
}

void pb_target::Set(playerent * entity)
{
	mTargetBot = entity;

	//entity does not have its own vec, need to convert from raw coords
	mTargetVec = entity->o;

	mTargetType = ETargetType::TARGET_TYPE_BOT;
}

void pb_marpo::AddTarget(pb_target * target)
{
	//assert that the target has a target position
	//assert(target->)

	switch (target->mTaskLevel)
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
	//Log the position in the circular collection
	mPreviousLocations.Add(target->mTargetVec);
}

void pb_marpo::PerformNextTask()
{
	//Check code supplied by base code
	mBot->pBot->CheckCrouch();
	mBot->pBot->CheckJump();

	//Start with the bot moving
	mBot->move = 1;

	CheckMorePertinentTasks();

	bool taskReady = false;
	while (!taskReady)
	{
		while (mCurrentTarget == nullptr || !mCurrentTarget->IsValid(mBot->pBot) || mCurrentTarget->IsCompleted(mBot->pBot))
		{
			DeleteCurrentTask();
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
				//Set the default target to not completed, this way it can be worked on again.		
				mDefaultTarget->Reset();
				mCurrentTarget = mDefaultTarget;
			}
		}

		//Check if there are any subtasks to perform
		if (mCurrentTarget->CalculateSubTasks(mBot->pBot)) {
			//Set the current to nullptr so when it loops around again we use the new task
			mCurrentTarget = nullptr;
			continue;
		}
		taskReady = true;
	}

	//We either need to perform the current task or a new one is given.
	debugbeam(mBot->o, mCurrentTarget->mTargetVec);
	mCurrentTarget->PerformTask(mBot->pBot);
}

void pb_marpo::ClearTasks()
{
	ClearTaskStack(TASK_LEVEL_IMMEDIATE);
	ClearTaskStack(TASK_LEVEL_REACTIVE);
	ClearTaskStack(TASK_LEVEL_LONGTERM);
}

void pb_marpo::ClearTaskStack(ETaskLevel taskLevel)
{
	switch (taskLevel)
	{
	case TASK_LEVEL_LONGTERM:
	{
		while (!mLongTermTasks.empty())
		{
			auto t = mLongTermTasks.top();
			mLongTermTasks.pop();
			delete t;
		}
	}
	break;
	case TASK_LEVEL_REACTIVE:
	{
		while (!mReactiveTasks.empty())
		{
			auto t = mReactiveTasks.top();
			mReactiveTasks.pop();
			delete t;
		}
	}
	break;
	case TASK_LEVEL_IMMEDIATE:
	{
		while (!mImmediateTasks.empty())
		{
			auto t = mImmediateTasks.top();
			mImmediateTasks.pop();
			delete t;
		}
	}
	break;
	default:
		break;
	}
}

void pb_marpo::CheckMorePertinentTasks()
{
	if (mCurrentTarget != nullptr)
	{
		if (mCurrentTarget->GetTaskLevel() == TASK_LEVEL_LONGTERM
			&& !mReactiveTasks.empty())
		{
			mCurrentTarget = mReactiveTasks.top();
			mReactiveTasks.pop();
		}
		if (mCurrentTarget->GetTaskLevel() == TASK_LEVEL_REACTIVE
			&& !mImmediateTasks.empty())
		{
			mCurrentTarget = mImmediateTasks.top();
			mImmediateTasks.pop();
		}
	}
}

void pb_marpo::DeleteCurrentTask()
{
	if (mCurrentTarget != mDefaultTarget)
		delete mCurrentTarget;
}
