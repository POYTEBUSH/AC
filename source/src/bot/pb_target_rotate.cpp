#include "cube.h"
#include "pb_target_rotate.h"

#include "pb_target_movement.h"

class pb_target_movement;

bool pb_target_rotate::CalculateSubTasks(CBot * bot)
{
	//No sub tasks for now
	return false;
}

void pb_target_rotate::PerformTask(CBot * bot)
{
	const int angleChunk = 360 / 10;
	
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
			break;
		}
	}

	bot->m_pMyEnt->move = 1;

	//auto botMarpo = pb_marpomanager::Instance().GetBotAttachment(bot->m_pMyEnt);
	//pb_target_movement* newMovementTask = new pb_target_movement(mTaskLevel);
	//////Get the bots current "fixed" forward vector
	////vec src = bot->GetViewAngles();
	////src.x = 0;
	////vec forward, right, up, dir, dest;
	////traceresult_s tr;

	////AnglesToVectors(src, forward, right, up);

	////const double  moveDistance = 10.0;

	////newMovementTask->Set(forward.mul(moveDistance));
	//botMarpo->AddTarget(newMovementTask);

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
