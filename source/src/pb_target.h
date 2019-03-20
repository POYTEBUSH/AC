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

enum ETaskLevel {
	TASK_LEVEL_LONGTERM,
	TASK_LEVEL_REACTIVE,
	TASK_LEVEL_IMMEDIATE
};


//The base target action for use with MARPO
class pb_target
{
public:
	pb_target(ETaskLevel taskLevel);
	~pb_target();

	void Set(const vec& position);
	void Set(const entity* entity);
	void Set(const playerent* entity);

	virtual void CalculateSubTasks() = 0;
	virtual void PerformTask() = 0;
	bool IsCompleted() const { return mIsCompleted; }

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

	//This tasks level, stored so we can add the sub-tasks to the same stack
	ETaskLevel mTaskLevel;

	//Is the task completed or not
	bool mIsCompleted;
};