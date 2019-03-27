#pragma once
#include "geom.h"
#include "entity.h"

#include "bot.h"

#include <vector>
#include <assert.h>

#include <map>

#include <stack>

//Forward Dec
class CBot;
class pb_marpomanager;


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

	///<summary>Return a collection of required sub-tasks</summary>
	virtual std::vector<pb_target*> CalculateSubTasks(CBot* bot) = 0;
	virtual void PerformTask(CBot* bot) = 0;
	virtual bool IsValid(CBot* bot) = 0;

	bool IsCompleted() const { return mIsCompleted; }

	ETaskLevel GetTaskLevel()const { return mTaskLevel; }

protected:

	//Position of target
	vec mTargetVec;

	//Target entity if targeting a collectable

	const entity* mTargetEntity = nullptr;
	//Target entity when targeting a bot or player
	const playerent* mTargetBot = nullptr;;

	//Type of target
	ETargetType mTargetType;

	//Range within where the target is regarded as obtained
	float mTargetCompletionRange;

	//This tasks level, stored so we can add the sub-tasks to the same stack
	ETaskLevel mTaskLevel;

	//Is the task completed or not
	bool mIsCompleted = false;

	int runtime = 0;
};

class pb_marpo
{
public:

	pb_marpo(botent* bot) : mBot(bot) {}
	~pb_marpo() {}

	void AddTarget(pb_target* target, ETaskLevel taskLevel);

	void SetDefaultTarget(pb_target* target) { mDefaultTarget = target; };

	void PerformNextTask();

private:
	friend class pb_marpomanager;

	std::stack<pb_target*> mLongTermTasks;
	std::stack<pb_target*> mReactiveTasks;
	std::stack<pb_target*> mImmediateTasks;

	//The goal which will always be performed
	pb_target* mDefaultTarget = nullptr;;

	//The current goal which the entity is attempting to perform
	pb_target* mCurrentTarget = nullptr;;

	//pointer the bot which this MARPO instance is managing
	botent* mBot = nullptr;;

};

class pb_marpomanager
{
public:

	static pb_marpomanager& Instance() {
		static pb_marpomanager instance;
		return instance;
	};

	pb_marpo* AttachBot(botent* bot)
	{
		if (mMarpoInstances.find(bot) == mMarpoInstances.end()) {
			mMarpoInstances[bot] = new pb_marpo(bot);
		}
		return mMarpoInstances[bot];
	}

	pb_marpo* GetBotAttachment(botent* bot)
	{
		if (mMarpoInstances.find(bot) != mMarpoInstances.end()) {
			return mMarpoInstances[bot];
		}
		return nullptr;
	}

	void ClearAttachments()
	{
		for (auto i : mMarpoInstances)
		{
			DetachBot(i.first);
		}
	}

	void DetachBot(botent* bot)
	{
		delete mMarpoInstances[bot];
	}

	void Think()
	{
		//for (auto i : mMarpoInstances) i->mBot->Think();
	}

private:
	pb_marpomanager() {};
	pb_marpomanager(pb_marpomanager const&);    // Don't Implement
	void operator=(pb_marpomanager const&);	// Don't implement

	std::map<botent*, pb_marpo*> mMarpoInstances;
	//std::vector<pb_marpo*> mMarpoInstances;
};