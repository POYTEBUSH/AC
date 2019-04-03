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

void pb_target::Set(botent * entity)
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

	switch (target->mTargetType)
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
	mBot->pBot->CheckStuck();
	mBot->pBot->CheckCrouch();
	mBot->pBot->CheckJump();
	mBot->pBot->CheckReload();

	CheckMorePertinentTasks();

	if (mCurrentTarget == nullptr || !mCurrentTarget->IsValid(mBot->pBot) || mCurrentTarget->IsCompleted(mBot->pBot))
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
			//Set the default target to not completed, this way it can be worked on again.
			mDefaultTarget->Reset();
			mCurrentTarget = mDefaultTarget;
		}
	}

	//Check if there are any subtasks to perform
	if (mCurrentTarget->CalculateSubTasks(mBot->pBot)) {
		//Set the current to nullptr so when it loops around again we use the new task
		mCurrentTarget = nullptr;
		return;
	}

	//We either need to perform the current task or a new one is given.
	mCurrentTarget->PerformTask(mBot->pBot);
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
