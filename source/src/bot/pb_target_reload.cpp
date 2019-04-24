#include "cube.h"
#include "pb_target_reload.h"

#include "pb_target_hunt.h"

bool pb_target_reload::CalculateSubTasks(CBot * bot)
{
	//If you have less than the required ammo that the bot requires to reload, find some
	//if (bot->m_pMyEnt->ammo[bot->m_pMyEnt->weaponsel->type] < WeaponInfoTable[bot->m_pMyEnt->weaponsel->type].sMinDesiredAmmo)
	//{
	//	pb_target_hunt* huntTask = new pb_target_hunt(mTaskLevel);
	//	//Set the tasks target to the current position
	//	huntTask->SetTargetType(EntityTypes::I_AMMO);
	//	pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt)->AddTarget(huntTask);
	//	return true;
	//}

	return false;
}

void pb_target_reload::PerformTask(CBot * bot)
{
	bot->CheckReload();
}

bool pb_target_reload::IsValid(CBot * bot)
{
	return true;
}

bool pb_target_reload::IsCompleted(CBot * bot)
{
	//Can assume that this confirms the reload, is used in reload func
	return (bot->m_pMyEnt->mag[bot->m_pMyEnt->weaponsel->type] >= WeaponInfoTable[bot->m_pMyEnt->weaponsel->type].sMinDesiredAmmo);
}
