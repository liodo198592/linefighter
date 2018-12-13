#ifndef _CClientStatusGame_H_
#define _CClientStatusGame_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CRoom.h"
#include "CPlayer.h"
#ifdef MAKE_ANDRIOD
#include "CClientStatus.h"
#else
#include "..\LFClient\CClientStatus.h"
#endif // MAKE_ANDRIOD
#include "ClientStatusGameBase.h"


class CFormatUnit;

class CClientStatusGame : public CClientStatusGameBase
{
public:

// 	int isActed;
// 	int isGameOver;
// 	int RoundIndex;
// 	int LastRoundIndex;


	CClientStatusGame(CRoom room_x,string userName_x);
	virtual ~CClientStatusGame(){}
	virtual void runStatus(int sigal){return;}

	virtual void showInfo();
	virtual void showMenu();


	virtual int getPlayerInfo();
	virtual int getLastActionInfo();//获取上轮所有玩家的动作
	virtual int getDetailInfo();//获取上轮所有动作结果的文字
	virtual int getFormatInfo();//获取上轮的演算结果
	virtual int stepPlayerInfo();
	virtual int getSendActions(vector<CAction>& subVec, int& RoundIndex);
	virtual int getMe(CPlayer& p);
	virtual int quitRoom(int roomID);

	//////////////////////////////////////////////////////////////////////////
	virtual string getTitleInfo();

	virtual CPlayer* getPlayerByName(string strName);
	//////////////////////////////////////////////////////////////////////////

	CRoom room;
// 	string userName;
// 	
// 	vector<CPlayer> vplayers;
// 	map<string ,vector<CAction> > vShowActions; //显示步骤
// 	map<int,vector<string> > vdiscreption;//动作号，语句
// 	map<int,vector<CFormatUnit *> > vformatdescription;//动作号，语句
// 	map<int,vector<CPlayer> > vdispPlayer;
	
	
protected:

	//CClientStatusGame * m_pCmdStatus;
};


#endif