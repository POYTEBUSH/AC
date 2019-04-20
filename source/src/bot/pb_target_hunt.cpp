#include "cube.h"
#include "pb_target_hunt.h"

#include "pb_target_movement.h"
#include "pb_target_rotate.h"

bool pb_target_hunt::CalculateSubTasks(CBot * bot)
{
	mFoundEntity = FindEntity(bot, mHuntType);

	if (mFoundEntity == nullptr)
	{
		//Use of memory on existing entity locations
		auto locationMem = pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->GetPickupLocationMemory();
		for (size_t i = 0; i < locationMem->Size(); i++)
		{
			double closestDist = 1e4;
			auto thisLoc = locationMem->At(i);
			if (bot->IsReachable(vec(thisLoc->x, thisLoc->y, thisLoc->z)) && thisLoc->type == mHuntType)
			{
				if (bot->GetDistance(vec(thisLoc->x, thisLoc->y, thisLoc->z)) < closestDist)
				{
					closestDist = bot->GetDistance(vec(thisLoc->x, thisLoc->y, thisLoc->z));
					mFoundEntity = thisLoc;
				}
			}
		}
	}
	//Now check if we have a location
	if (mFoundEntity != nullptr)
	{
		mTaskFound = true;
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->GetPickupLocationMemory()->Add(mFoundEntity);

		pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
		newMovementTask->Set(mFoundEntity);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(newMovementTask);

		return true;
	}
	mTaskFound = false;

	if (!mRotationPerformed) {
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(this);

		pb_target_rotate* rotateTarget = new pb_target_rotate(mTaskLevel);
		pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(rotateTarget);
		mRotationPerformed = true;
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
		if (e.type == type && bot->IsInFOV(vec(e.x,e.y,e.z)) && bot->IsReachable(vec(e.x, e.y, e.z)))
			return &ents[i];
	}
	return nullptr;
}
