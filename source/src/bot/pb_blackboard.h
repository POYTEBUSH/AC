#pragma once
#include <vector>
#include <map>

#include "bot.h"

enum pb_blackboard_query_type
{
	Health,
	Ammo,
	Enemy,
	Armour
};

struct pb_blackboard_query
{
	botent* Sender;
	pb_blackboard_query_type QueryType;
	bool Completed;
	
	pb_blackboard_query(botent* s, pb_blackboard_query_type type)
	{
		Sender = s;
		QueryType = type;
		Completed = false;
	}
	pb_blackboard_query() {}
};

class pb_blackboard
{
public:

	void AddQuery(playerent* a, pb_blackboard_query query);

	void AddPickupInfo(entity* ent, pb_blackboard_query_type type);
	void AddEnemyInfo(playerent* ent);

	void PerformArbitration();

private:
	
	std::map<playerent*, pb_blackboard_query> mQueries;

	//Used to store the last known location, rather than the actual.
	//When sent to a player their information is removed
	//Map is used to ensure data is not spammed
	std::map<entity*, vec> mHealthPickups;
	std::map<entity*, vec> mAmmoPickups;
	std::map<entity*, vec> mArmourPickups;
	std::map<playerent*, vec> mEnemies;
};

constexpr int MAX_TEAMS = 2;

class pb_blackboard_manager
{
public:

	static pb_blackboard_manager* Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new pb_blackboard_manager();
			mInstance->Init();
		}
		return mInstance;
	}

	void Init();

	void AddQuery(playerent* a, pb_blackboard_query query, int team);

	void AddPickupInfo(entity* ent, int team);
	void AddEnemyInfo(playerent* ent, int team);

	void PerformArbitrations();

private:


	pb_blackboard_manager() {};
	~pb_blackboard_manager() {};

	static pb_blackboard_manager* mInstance;

	pb_blackboard* mBlackboards[MAX_TEAMS];
};

