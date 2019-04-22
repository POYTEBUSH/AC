#pragma once

#include "pb_marpo.h"
#include "entity.h"
#include "bot.h"

#include "pb_FuzzyModule.h"
#include "pb_FuzzyTerm.h"

#include "tools.h"

#include <sstream>	

class pb_botmanager
{

public:

	static pb_botmanager* Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new pb_botmanager();
			mInstance->Init();
		}
		return mInstance;
	}

	void Init();
	void Update(vector<botent*> bots);

private:

	pb_botmanager() {};
	~pb_botmanager() {};

	pb_FuzzyModule mFuzzyModule;

	static pb_botmanager* mInstance;

	//Copy of the weapons info table, not ideal but is due to complicated setup of starting project
	weaponinfo_s WeaponInfoTable[MAX_WEAPONS] =
	{
		// DD: desired distance
		// FD: fire distance
		// mA: min desired Ammo
		// ---- Type : -- minDD -- maxDD -- minFD -- maxFD -- mA
		{ TYPE_MELEE,      0.0f,    5.0f,   0.0f,    3.5f,   0 }, // KNIFE
		{ TYPE_NORMAL,     3.5f,   15.0f,   3.0f,   60.0f,   6 }, // PISTOL
		{ TYPE_SNIPER,    15.0f,   80.0f,   3.0f,  180.0f,   2 }, // CARBINE    // min and max FD just guesstimates for now
		{ TYPE_SHOTGUN,    0.0f,   15.0f,   0.0f,   50.0f,   3 }, // SHOTGUN
		{ TYPE_AUTO,       0.0f,   25.0f,   3.0f,   60.0f,  10 }, // SUBGUN
		{ TYPE_SNIPER,    10.0f,  125.0f,   3.0f,  200.0f,   3 }, // SNIPER
		{ TYPE_AUTO,      20.0f,   80.0f,   3.0f,  150.0f,   6 }, // ASSAULT
		{ TYPE_AUTO,       0.0f,    0.0f,   3.0f,    0.0f,   6 }, // CPISTOL
		{ TYPE_GRENADE,    5.0f,   30.0f,   3.0f,   50.0f,   1 }, // GRENADE
		{ TYPE_AUTO,       0.0f,   50.0f,   3.0f,   50.0f,   0 }  // AKIMBO
	};

	itemstat ammostats[NUMGUNS] =
	{
		{ 1,  1,   1,  S_ITEMAMMO },   // knife dummy
	{ 20, 60, 100,  S_ITEMAMMO },   // pistol
	{ 15, 30,  30,  S_ITEMAMMO },   // carbine
	{ 14, 28,  21,  S_ITEMAMMO },   // shotgun
	{ 60, 90,  90,  S_ITEMAMMO },   // subgun
	{ 10, 20,  15,  S_ITEMAMMO },   // sniper
	{ 40, 60,  60,  S_ITEMAMMO },   // assault
	{ 30, 45,  75,  S_ITEMAMMO },   // cpistol
	{ 1,  0,   3,  S_ITEMAMMO },   // grenade
	{ 100,  0, 100,  S_ITEMAKIMBO }    // akimbo
	};
};

