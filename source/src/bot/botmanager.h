#pragma once
#include "bot/bot.h"
#include "pb_marpo.h"
#include "pb_target_movement.h"

class CBotManager
{
	char m_szBotNames[150][16]; // Max 150 bot names with a length of 16 characters
	short m_sBotNameCount;
	char m_szBotTeams[20][5]; // Max 100 bot teams co a length of 5 characters
	short m_sBotTeamCount;
	bool m_bInit;
	bool m_bBotsShoot;
	bool m_bIdleBots;
	bot_skill_s m_BotSkills[5]; // 5 different bot skills, 0 = best 4 = worst
	int m_iFrameTime;
	int m_iPrevTime;
	short m_sBotSkill; // Bad - Worse - Medium - Good - Best
	short m_sMaxAStarBots; // Max bots that can use a* at the same time
	short m_sUsingAStarBotsCount; // Number of bots that are using a*
	short m_sCurrentTriggerNr; // Current waypoint trigger bots should use
	TLinkedList<CStoredBot *> m_StoredBots; // List of bots that should be re-added after map change
	float m_fReAddBotDelay;

	void LoadBotNamesFile(void);
	const char *GetBotName(void);
	void LoadBotTeamsFile(void);
	void CreateSkillData(void);
	void InitSkillData(void);
	void InitBotItems(void);

	friend class CBot;
	friend class CACBot;
	friend class CWaypointClass;

	//Additionals - b013432f
	std::vector<pb_marpo*> mMARPO_Managers;

public:
	botent *m_pBotToView;

	// Construction
	CBotManager(void) { m_bInit = true; m_bBotsShoot = true; m_bIdleBots = false; m_fReAddBotDelay = -1.0f; };

	// Destruction
	~CBotManager(void);

	void Init(void);
	void Think(void);
	void RenderBots(void);
	void RespawnBots(void) { loopv(bots) if (bots[i] && bots[i]->pBot) bots[i]->pBot->Spawn(); };
	void ClearStoredBots(void) { while (!m_StoredBots.Empty()) delete m_StoredBots.Pop(); }
	void ReAddBot(CStoredBot *bot) { CreateBot(bot->m_szTeam, SkillNrToSkillName(bot->m_sSkillNr), bot->m_szName); };
	void EndMap(void);
	void BeginMap(const char *szMapName);
	int GetBotIndex(botent *m);
	void LetBotsUpdateStats(void);
	void LetBotsHear(int n, vec *loc);
	void AddWaypoint(node_s *pNode);
	void DelWaypoint(node_s *pNode);
	bool BotsShoot(void) { return m_bBotsShoot; };
	bool IdleBots(void) { return m_bIdleBots; };
	void SetBotsShoot(bool bShoot) { m_bBotsShoot = bShoot; };
	void SetIdleBots(bool bIdle) { m_bIdleBots = bIdle; };
	void EnableBotView(botent *bot) { m_pBotToView = bot; };
	void DisableBotView(void);
	void ChangeBotSkill(short Skill, botent *bot = NULL);
	botent *CreateBot(const char *team, const char *skill, const char *name);
	void ViewBot(void);
	void CalculateMaxAStarCount(void);
	void PickNextTrigger(void);
	const char *GetBotTeam(void);

	void MakeBotFileName(const char *szFileName, const char *szDir1, const char *szDir2,
		char *szOutput);
};

