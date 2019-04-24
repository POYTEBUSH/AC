#include "cube.h"
#include "pb_botmanager.h"

//MARPO Tasks
#include "pb_target_attack.h"
#include "pb_target_rotate.h"

#include "pb_blackboard.h"

pb_botmanager* pb_botmanager::mInstance;

void pb_botmanager::Init()
{
	pb_FuzzyVariable& targetDistance = mFuzzyModule.CreateFuzzyVariable("TargetDistance");
	pb_FzSet targetClose = targetDistance.AddFuzzySet(FuzzySetType::LeftShoulder, "TargetClose", 0, 10, 20);
	pb_FzSet targetNear = targetDistance.AddFuzzySet(FuzzySetType::Triangular, "TargetNear", 10, 20, 50);
	pb_FzSet targetFar = targetDistance.AddFuzzySet(FuzzySetType::RightShoulder, "TargetFar", 20, 50, 1000);

	pb_FuzzyVariable& desirabilityScore = mFuzzyModule.CreateFuzzyVariable("Desirability");
	pb_FzSet desirabilityMinimal = desirabilityScore.AddFuzzySet(FuzzySetType::LeftShoulder, "MinimalDesirability", 0, 25, 50);
	pb_FzSet desirabilityNeutral = desirabilityScore.AddFuzzySet(FuzzySetType::Triangular, "NeutralDesirability", 25, 50, 75);
	pb_FzSet desirabilityFull = desirabilityScore.AddFuzzySet(FuzzySetType::RightShoulder, "FullDesirability", 50, 75, 100);

	mFuzzyModule.AddRule(targetClose, desirabilityFull);
	mFuzzyModule.AddRule(targetNear, desirabilityNeutral);
	mFuzzyModule.AddRule(targetFar, desirabilityMinimal);

	pb_FuzzyAttackCalc::Instance()->Init();
	pb_blackboard_manager::Instance()->Init();
}

void pb_botmanager::Update(vector<botent*> bots)
{
	pb_blackboard_manager::Instance()->PerformArbitrations();

	for (size_t i = 0; i < bots.length(); i++)
	{
		auto thisBotEnt = bots[i];
		auto thisBot = bots[i]->pBot;


		if (!bots[i]) continue;
		if (bots[i]->pBot)
		{
			auto botMarpoI = pb_marpomanager::Instance().GetBotAttachment(thisBotEnt);
			
			if (thisBotEnt->state == CS_ALIVE) {

				//Check the desirability of the current task
				auto currentTarget = botMarpoI->GetCurrentTarget();
				double currentBestScore = 0.0;
				if (currentTarget != nullptr)
				{					
					//If bot target, get that score... else use target distance scoring
					if (currentTarget->GetTargetType() == ETargetType::TARGET_TYPE_BOT)
					{
						pb_FuzzyAttackCalc::Instance()->CalculateDesirability(thisBotEnt, currentTarget->GetTargetPos());
						currentBestScore = mFuzzyModule.DeFuzzifyMaxAv("Desirability");
					}
					else
					{
						double targetDist = thisBot->GetDistance(currentTarget->GetTargetPos());
						mFuzzyModule.Fuzzify("TargetDistance", targetDist);
						currentBestScore = mFuzzyModule.DeFuzzifyMaxAv("Desirability");
					}
				}


				playerent* target = nullptr;

				double targetBestScore = 0.0;

				int weapon = thisBotEnt->weaponsel->type;
				std::stringstream ss;
				ss << "Weapon_";
				ss << weapon;

				////Don't bother looking to attack a bot if you have no ammo for it
				//if (thisBot->m_pMyEnt->mag[thisBot->m_pMyEnt->weaponsel->type] > 0)
				//{
					for (size_t j = 0; j < bots.length(); j++)
					{
						botent* enemyBot = bots[j];
						if (enemyBot->state == CS_ALIVE) {
							if (thisBot->IsInFOV(enemyBot) && (enemyBot->team != thisBotEnt->team || m_arena))
							{
								double desireToAttack = pb_FuzzyAttackCalc::Instance()->CalculateDesirability(thisBotEnt, enemyBot);

								if (desireToAttack > targetBestScore)
								{
									targetBestScore = desireToAttack;
									target = enemyBot;
								}
							}
						}
					}
					if (player1->state == CS_ALIVE) {
						if (thisBot->IsInFOV(player1) && (player1->team != thisBotEnt->team || m_arena))
						{
							double desireToAttack = pb_FuzzyAttackCalc::Instance()->CalculateDesirability(thisBotEnt, player1);

							if (desireToAttack > targetBestScore)
							{
								targetBestScore = desireToAttack;
								target = player1;
							}
						}
					}
				

				//Check if we have a new target and the new target has a higher desirability,
				//The new target must either be more desirable, no current target or a higher risk
				if (target != nullptr && 
					(targetBestScore > currentBestScore 
						|| currentTarget == nullptr 
						|| (currentTarget->GetTaskLevel() < TASK_LEVEL_REACTIVE)))
				{
					auto attackTask = new pb_target_attack(TASK_LEVEL_REACTIVE);
					attackTask->Set(target);
					botMarpoI->AddTarget(attackTask);
				}
				else
				{
					//Doesn't currently create sub tasks
					if (thisBot->m_pMyEnt->enemy == nullptr) {
						if (thisBot->m_iStuckCheckDelay + (thisBot->CheckCrouch() ? 2000 : 0) >= lastmillis)
							break;

						bool IsStuck = false;
						vec CurPos = thisBotEnt->o, PrevPos = thisBot->m_vPrevOrigin;
						CurPos.z = PrevPos.z = 0;
						// Did the bot hardly move the last frame?
						if (GetDistance(CurPos, PrevPos) <= 0.1f)
						{
							if (thisBot->m_bStuck)
							{
								if (thisBot->m_iStuckTime < lastmillis)
									IsStuck = true;
							}
							else
							{
								thisBot->m_bStuck = true;
								thisBot->m_iStuckTime = lastmillis + 1000;
							}
						}
						else
						{
							thisBot->m_bStuck = false;
							thisBot->m_iStuckTime = 0;
						}

						if (IsStuck)
						{
							pb_target_rotate* rotationTask = new pb_target_rotate(TASK_LEVEL_REACTIVE);
							pb_marpomanager::Instance().GetBotAttachment(thisBot->m_pMyEnt)->AddTarget(rotationTask);
						}
					}

				}
			}
			thisBot->Think();

			auto newCurrentTarget = botMarpoI->GetCurrentTarget();
			if (newCurrentTarget != nullptr)
			{
				auto newAim = newCurrentTarget->GetLookAtTarget(thisBot);
				if (newAim != vec(-1, -1, -1))
				{
					thisBot->AimToVec(newAim);
				}
				else
				{
					thisBotEnt->pitch = 1;
					thisBot->AimToIdeal();
				}
			}
		}
	}	
}