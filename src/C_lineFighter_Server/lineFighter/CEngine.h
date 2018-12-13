#ifndef _CEngine_H_
#define _CEngine_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "RingBuf.h"
#include "CProtocol.h"
#include "CUser.h"
#include "CRoom.h"

class CTCPServer;
class CPlayer;
class CAction;
class CUser;

class CEngine
{
private:

public:
    CEngine(vector<CUser> vecUser_x , int roomID_x)
	{
		vecUser = vecUser_x;
		RoundIndex = 0;
		isGameOver = -1;
		iTimeOutPerRound = 600;//30秒超时
		roomID = roomID_x;
		roomStatus = EROOM_STATUS_GAMING;
	}
	CEngine()
	{
		RoundIndex = 0;
		isGameOver = -1;
		iTimeOutPerRound = 600;//30秒超时
		roomStatus = EROOM_STATUS_GAMING;
		roomID = 1;
	}
    
	virtual ~CEngine() 	{}

	int waitFor(int dest);
	//根据当前的player状态和当前的Action状态来决定后续的player
	int calcARound(vector<CPlayer>& vplayers,map<string,vector<CAction> >& vActions, 
		map<int,vector<string> >& vdiscreption , map<int,vector<string> >& vFormatdiscreption,map<int,vector<CPlayer> >& vLivedPlayers);
	
	int initGame(vector<CUser> vecUser);

    int checkGameNo();
    void sendPlayerInfoAndCheck(vector<CPlayer> vplayers);
    void sendDescripAndCheck(int RoundIndex, map<int,vector<string> > vdiscreption);
    void sendGameOverAndCheck(string winnerName,int RoundIndex,int iPlayerID);
	void setPlayers(vector<CPlayer> vplayers_x);
	int commitAction(vector<CAction>& subVec,int& RoundIndex_x, string userName);

	void waitForAction(int timeout);
	int findIndexPLbyName(string name);
	int svc();

public:
	EROOM_STATUS roomStatus;
	vector<CPlayer> vplayers;
	int RoundIndex; //1000表示游戏结束
	map<string ,vector<CAction> > vActions; //ACTION 编号与 vecUser编号相同，电脑总是补充在最后
	map<string ,vector<CAction> > vShowActions; //ACTION 编号与 vecUser编号相同，电脑总是补充在最后
	map<int,vector<string> > vdiscreption;//动作号，语句
	map<int,vector<string> > vFormatdiscreption;//动作号，语句
	map<int,vector<CPlayer> > vLivedPlayers;//队伍号，玩家
	map<int,vector<CPlayer> > vdispPlayer;
	vector<CUser> vecUser;//ACTION 编号与 vecUser编号相同，电脑总是补充在最后
	int lifeNum ; //存活的玩家数量
	int iTimeOutPerRound;//每一回合的超时时间
	string winnerinfo;
	int isGameOver;
	int roomID;

private:
	
};






#endif