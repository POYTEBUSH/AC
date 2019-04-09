#include "cube.h"
#include "pb_target_reload.h"

bool pb_target_reload::CalculateSubTasks(CBot * bot)
{
	//If you have less than the required ammo that the bot requires, find some
	//if (bot->m_pMyEnt->mag[bot->m_pMyEnt->weaponsel->type] < WeaponInfoTable[bot->m_pMyEnt->weaponsel->type].sMinDesiredAmmo)


	return false;
}

void pb_target_reload::PerformTask(CBot * bot)
{
	bot->CheckReload();
}

bool pb_target_reload::IsValid(CBot * bot)
{
	return IsCompleted(bot);
}

bool pb_target_reload::IsCompleted(CBot * bot)
{
	//Can assume that this confirms the reload, is used in reload func
	return (bot->m_pMyEnt->mag[bot->m_pMyEnt->weaponsel->type] >= WeaponInfoTable[bot->m_pMyEnt->weaponsel->type].sMinDesiredAmmo);
}
