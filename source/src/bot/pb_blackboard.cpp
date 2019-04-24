#include "cube.h"
#include "pb_blackboard.h"

#include "pb_target_movement.h"

#include <assert.h>

pb_blackboard_manager* pb_blackboard_manager::mInstance;

void pb_blackboard_manager::Init()
{
	for (size_t i = 0; i < MAX_TEAMS; i++)
	{
		mBlackboards[i] = new pb_blackboard();
	}
}

void pb_blackboard_manager::AddQuery(playerent* a, pb_blackboard_query query, int team)
{
	assert(mBlackboards[team] != nullptr || m_arena);

	mBlackboards[team]->AddQuery(a, query);
}

void pb_blackboard_manager::AddPickupInfo(entity * ent, int team)
{
	assert(mBlackboards[team] != nullptr || m_arena);

	if (ent->type == EntityTypes::I_HEALTH)
		mBlackboards[team]->AddPickupInfo(ent, pb_blackboard_query_type::Health);

	if (ent->type == EntityTypes::I_AMMO)
		mBlackboards[team]->AddPickupInfo(ent, pb_blackboard_query_type::Ammo);

	if (ent->type == EntityTypes::I_ARMOUR)
		mBlackboards[team]->AddPickupInfo(ent, pb_blackboard_query_type::Armour);
}

void pb_blackboard_manager::AddEnemyInfo(playerent * ent, int team)
{
	assert(mBlackboards[team] != nullptr || m_arena);

	mBlackboards[team]->AddEnemyInfo(ent);
}

void pb_blackboard_manager::PerformArbitrations()
{
	for (auto b : mBlackboards)
	{
		b->PerformArbitration();
	}
}

void pb_blackboard::AddQuery(playerent* a, pb_blackboard_query query)
{
	mQueries[a] = query;
}

void pb_blackboard::AddPickupInfo(entity * ent, pb_blackboard_query_type type)
{
	assert(type >= 0 && type < 2);

	switch (type)
	{
	case Health:
	{
		mHealthPickups[ent] = vec(ent->x, ent->y, ent->z);
	}
	break;
	case Ammo:
	{
		mAmmoPickups[ent] = vec(ent->x, ent->y, ent->z);
	}
	break;
	case Armour:
	{
		mArmourPickups[ent] = vec(ent->x, ent->y, ent->z);
	}
	case Enemy:
		break;
	default:
		break;
	}
}

void pb_blackboard::AddEnemyInfo(playerent * ent)
{
	mEnemies[ent] = ent->o;
}

void pb_blackboard::PerformArbitration()
{
	for (auto q : mQueries)
	{
		double closestDist = 1e4;
		bool found = false;
		vec targetPos;
		entity* foundEnt = nullptr;

		switch (q.second.QueryType)
		{
		case Health:
		{
			for (auto hp : mHealthPickups)
			{
				double dist = q.second.Sender->o.dist(hp.second);
				if (dist < closestDist)
				{
					closestDist = dist;
					targetPos = hp.second;
					foundEnt = hp.first;
					found = true;
				}
			}
			if (found)
			{
				pb_marpomanager::Instance().GetBotAttachment(q.second.Sender)->GetPickupLocationMemory()->Add(foundEnt);
				mHealthPickups.erase(foundEnt);
			}
		}
		break;
		case Ammo:
		{
			for (auto hp : mAmmoPickups)
			{
				double dist = q.second.Sender->o.dist(hp.second);
				if (dist < closestDist)
				{
					closestDist = dist;
					targetPos = hp.second;
					foundEnt = hp.first;
					found = true;
				}
			}
			if (found)
			{
				pb_marpomanager::Instance().GetBotAttachment(q.second.Sender)->GetPickupLocationMemory()->Add(foundEnt);
				mAmmoPickups.erase(foundEnt);
			}
		}
		break;
		case Armour:
		{
			for (auto hp : mArmourPickups)
			{
				double dist = q.second.Sender->o.dist(hp.second);
				if (dist < closestDist)
				{
					closestDist = dist;
					targetPos = hp.second;
					foundEnt = hp.first;
					found = true;
				}
			}
			if (found)
			{
				pb_marpomanager::Instance().GetBotAttachment(q.second.Sender)->GetPickupLocationMemory()->Add(foundEnt);
				mArmourPickups.erase(foundEnt);
			}
		}
		case Enemy:
		{
			playerent* foundEnt;
			for (auto hp : mEnemies)
			{
				double dist = q.second.Sender->o.dist(hp.second);
				if (dist < closestDist)
				{
					closestDist = dist;
					targetPos = hp.second;
					foundEnt = hp.first;
					found = true;
				}
			}
			if (found)
			{
				mEnemies.erase(foundEnt);
			}
		}
		break;
		default:
			break;
		}

		if (found)
		{
			pb_target_movement* newMovementTask = new pb_target_movement(TASK_LEVEL_REACTIVE);

			if (foundEnt != nullptr)
				newMovementTask->Set(foundEnt);
			else
				newMovementTask->Set(targetPos);


			pb_marpomanager::Instance().GetBotAttachment(q.second.Sender)->AddTarget(newMovementTask);
			q.second.Completed = true;
		}
	}

	auto i = begin(mQueries);

	while (i != end(mQueries)) {
		if (i->second.Completed)
			mQueries.erase(i);
		else
			++i;
	}

}
