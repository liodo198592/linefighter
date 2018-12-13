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
		iTimeOutPerRound = 600;//30�볬ʱ
		roomID = roomID_x;
		roomStatus = EROOM_STATUS_GAMING;
	}
	CEngine()
	{
		RoundIndex = 0;
		isGameOver = -1;
		iTimeOutPerRound = 600;//30�볬ʱ
		roomStatus = EROOM_STATUS_GAMING;
		roomID = 1;
	}
    
	virtual ~CEngine() 	{}

	int waitFor(int dest);
	//���ݵ�ǰ��player״̬�͵�ǰ��Action״̬������������player
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
	int RoundIndex; //1000��ʾ��Ϸ����
	map<string ,vector<CAction> > vActions; //ACTION ����� vecUser�����ͬ���������ǲ��������
	map<string ,vector<CAction> > vShowActions; //ACTION ����� vecUser�����ͬ���������ǲ��������
	map<int,vector<string> > vdiscreption;//�����ţ����
	map<int,vector<string> > vFormatdiscreption;//�����ţ����
	map<int,vector<CPlayer> > vLivedPlayers;//����ţ����
	map<int,vector<CPlayer> > vdispPlayer;
	vector<CUser> vecUser;//ACTION ����� vecUser�����ͬ���������ǲ��������
	int lifeNum ; //�����������
	int iTimeOutPerRound;//ÿһ�غϵĳ�ʱʱ��
	string winnerinfo;
	int isGameOver;
	int roomID;

private:
	
};






#endif