#include "cube.h"
#include "pb_target_hunt.h"

#include "pb_target_movement.h"

bool pb_target_hunt::CalculateSubTasks(CBot * bot)
{
	entity* foundEntity = FindEntity(bot, mHuntType);

	if (foundEntity != nullptr)
	{
		mTaskFound = true;

		pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
		newMovementTask->Set(foundEntity);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(newMovementTask);

		return true;
	}
	return false;
}

void pb_target_hunt::PerformTask(CBot * bot)
{
	//No direct task for this action, it is performed as part of the calculate sub tasks
}

bool pb_target_hunt::IsValid(CBot * bot)
{
	//Could check if the bot still requires the pickup
	return true;
}

bool pb_target_hunt::IsCompleted(CBot * bot)
{
	return mTaskFound;
}

entity* pb_target_hunt::FindEntity(CBot * bot, EntityTypes type)
{
	loopv(ents)
	{
		auto &e = ents[i];
		//Check if the entity is one that is required and if it's in the bots view
		if (e.type == type && bot->IsInFOV(vec(e.x,e.y,e.z)))
			return &ents[i];
	}
	return nullptr;
}
