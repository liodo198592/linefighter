#ifndef _STATE_HANDLER_H_
#define _STATE_HANDLER_H_
#ifdef MAKE_ANDRIOD
#include "ClientStatusGameBase.h"
#else
#include "LFClient\ClientStatusGameBase.h"
#endif
#include "ShowScreen.h"
#include "MenuControler.h"

enum GameStat
{
	GAMESTAT_WAIT_INIT,
	GAMESTAT_PLAYER_INPUT,
	GAMESTAT_WAITOTHERINPUT,
	GAMESTAT_SHOWING,
	GAMESTAT_OVER,
};

class CStateHandler
{
public:
	CStateHandler(CClientStatusGameBase* pCmdStatus,CShowScreen* pShowScreen,
		CMenuControler* pMenuControl,GameStat startState=GAMESTAT_WAIT_INIT);
	~CStateHandler(void);

	void handleState(float tmd);

	GameStat getCurrentState();

private:

	//���ز�����ť
	void hidMenuButton();
	//��ʾ������ť
	void showMenuButton();

	void changeStat(GameStat toStat);

private:
	GameStat m_CurrentStat;

	CShowScreen* m_pShowScreen;

	CClientStatusGameBase* m_pCmdStatus;

	CMenuControler* m_pMenuControl;
};

#endif