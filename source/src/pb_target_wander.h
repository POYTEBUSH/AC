#pragma once
#include "pb_target.h"

class pb_target_wander : public pb_target
{
public:

	pb_target_wander(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_wander() {};

	std::vector<pb_target*> CalculateSubTasks(CBot* bot) {

		auto output = std::vector<pb_target*>();
		auto subtask = new pb_target_movement(mTaskLevel);

		bot->m_pMyEnt->pitch = 0;

		//Get a nearby possition, set a sub task as a target
		auto targetPos = bot->GetNearestWaypoint(10.f);
		subtask->Set(targetPos->pNode->v_origin);

		output.push_back(subtask);
	};

	//This is a dud target which creates a sub task
	void PerformTask(CBot* bot) {
		mIsCompleted = true;
	}

};

