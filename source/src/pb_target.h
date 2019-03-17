#pragma once
#include "geom.h"
#include "entity.h"

//Forward Dec
class CBot;

enum ETargetType 
{
	TARGET_TYPE_ENTITY,
	TARGET_TYPE_BOT,
	TARGET_TYPE_NONE
};

class pb_target
{
public:
	pb_target();
	~pb_target();

	void Set(const vec& position);
	void Set(const entity* entity);
	void Set(const playerent* entity);

private:

	//Position of target
	vec mTargetVec;

	//Target entity if targeting a collectable
	const entity* mTargetEntity;
	//Target entity when targeting a bot or player
	const playerent* mTargetBot;

	//Type of target
	ETargetType mTargetType;

	//Range within where the target is regarded as obtained
	float mTargetCompletionRange;
};

class pb_task
{
public:
	pb_task(CBot* performingEntity) : mTaskLevel(TASK_LEVEL_LONGTERM), mPerformingEntity(performingEntity) {};
	pb_task(CBot* performingEntity, ETaskLevel taskLevel) : mTaskLevel(taskLevel), mPerformingEntity(performingEntity) {};
	~pb_task() {};

	virtual void CalculateSubTasks();
	virtual void PerformTask();
	bool IsCompleted() const { return mIsCompleted; }

protected:

	//This tasks level, stored so we can add the sub-tasks to the same stack
	ETaskLevel mTaskLevel;
	pb_target* mTarget;

	CBot* mPerformingEntity;

	bool mIsCompleted;
};

class pb_task_movement : public pb_task
{
public:

	void SetTarget(pb_target* target)
	{
		mTarget = target;
	};
	void PerformTask()
	{

	}
private:

};

class pb_task_wander : public pb_task
{
public:

	void CalculateSubTasks() {};
	void PerformTask() {};

private:


};