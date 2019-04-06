#pragma once
#include "pb_marpo.h"

class pb_target_reload : public pb_target
{
public:

	pb_target_reload(ETaskLevel taskLevel) : pb_target(taskLevel) {};
	~pb_target_reload() {};

	bool CalculateSubTasks(CBot* bot);
	void PerformTask(CBot* bot);
	bool IsValid(CBot* bot);
	bool IsCompleted(CBot* bot);

private:
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
};

