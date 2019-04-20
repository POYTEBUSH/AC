#include "cube.h"
#include "pb_target_rotate.h"

bool pb_target_rotate::CalculateSubTasks(CBot * bot)
{
	//No sub tasks for now
	return false;
}

void pb_target_rotate::PerformTask(CBot * bot)
{
	const int angleChunk = 360 / 10;

	//// Check the left side...
	//dir = right;
	//dest = bot->m_pMyEnt->o;
	//dir.mul(3);
	//dest.sub(dir);

	//TraceLine(bot->m_pMyEnt->o, dest, bot->m_pMyEnt, false, &tr);

	//if (!tr.collided)
	//{
	//	// Bot can turn left, do so
	//	bot->m_pMyEnt->targetyaw = WrapYZAngle(bot->m_pMyEnt->yaw - 90);
	//	bot->m_iStuckCheckDelay = bot->m_iCheckEnvDelay = lastmillis + 500;
	//}

	//// Check the right side...
	//dir = right;
	//dest = bot->m_pMyEnt->o;
	//dir.mul(3);
	//dest.add(dir);

	//TraceLine(bot->m_pMyEnt->o, dest, bot->m_pMyEnt, true, &tr);

	//if (!tr.collided)
	//{
	//	// Bot can turn right, do so
	//	bot->m_pMyEnt->targetyaw = WrapYZAngle(bot->m_pMyEnt->yaw + 90);
	//	bot->m_iStuckCheckDelay = bot->m_iCheckEnvDelay = lastmillis + 500;
	//}

	//// Check if bot can turn 180 degrees
	//dir = forward;
	//dest = bot->m_pMyEnt->o;
	//dir.mul(3);
	//dest.add(dir);

	//TraceLine(bot->m_pMyEnt->o, dest, bot->m_pMyEnt, true, &tr);

	//if (!tr.collided)
	//{
	//	// Bot can turn around, do so
	//	bot->m_pMyEnt->targetyaw = WrapYZAngle(bot->m_pMyEnt->yaw + 180);
	//	bot->m_iStuckCheckDelay = bot->m_iCheckEnvDelay = lastmillis + 500;
	//}
	
	//Utilising improved stuck check code
	for (size_t i = 0; i < 360; i+= angleChunk)
	{
		bot->m_pMyEnt->yaw = WrapYZAngle(bot->m_pMyEnt->yaw + i);

		vec src = bot->GetViewAngles();
		src.x = 0;
		vec forward, right, up, dir, dest;
		traceresult_s tr;

		AnglesToVectors(src, forward, right, up);

		dir = forward;
		dest = bot->m_pMyEnt->o;
		dir.rotate(i, dir);
		dir.mul(3);
		dest.add(dir);

		TraceLine(bot->m_pMyEnt->o, dest, bot->m_pMyEnt, true, &tr);
		if (!tr.collided)
		{
			//Bot can turn this direction, so do so.
			bot->m_pMyEnt->targetyaw = bot->m_pMyEnt->yaw;
			bot->m_iStuckCheckDelay = bot->m_iCheckEnvDelay = lastmillis + 500;
		}
	}
	mCompleted = true;
}

bool pb_target_rotate::IsValid(CBot * bot)
{
	return true;
}

bool pb_target_rotate::IsCompleted(CBot * bot)
{
	return mCompleted;
}
