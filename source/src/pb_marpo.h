#pragma once
#include "pb_target.h"

#include <stack>

//Forward Dec
class CBot;

class pb_marpo
{
public:

	pb_marpo(CBot* bot) : mBot(bot) {}
	~pb_marpo() {}

	void AddTarget(pb_target* target, ETaskLevel taskLevel);
	void SetDefaultTarget(pb_target* target) { mDefaultTarget = target; };

	void PerformNextTask();

private:

	std::stack<pb_target*> mLongTermTasks;
	std::stack<pb_target*> mReactiveTasks;
	std::stack<pb_target*> mImmediateTasks;

	//The goal which will always be performed
	pb_target* mDefaultTarget;

	//The current goal which the entity is attempting to perform
	pb_target* mCurrentTarget;

	//pointer the bot which this MARPO instance is managing
	CBot* mBot;
};
