#ifndef _CClientStatusSingle_H_
#define _CClientStatusSingle_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CRoom.h"
#include "CPlayer.h"
#ifdef MAKE_ANDRIOD
#include "CClientStatus.h"
#include "CEngine.h"
#else
#include "..\LFClient\CClientStatus.h"
#include "..\..\C_lineFighter_Server\lineFighter\CEngine.h"
#endif // MAKE_ANDRIOD

#include "ClientStatusGameBase.h"

class CFormatUnit;

class CClientStatusSingle : public CClientStatusGameBase
{
public:

// 	int isActed;
// 	int isGameOver;
// 	int RoundIndex;
// 	int LastRoundIndex;


	CClientStatusSingle(CEngine * pEngine_x,CRoom room_x,string userName_x);
	virtual ~CClientStatusSingle(){}
	virtual void runStatus(int sigal){return;}

	virtual void showInfo();
	virtual void showMenu();


	virtual int getPlayerInfo();
	virtual int getLastActionInfo();//��ȡ����������ҵĶ���
	virtual int getDetailInfo();//��ȡ�������ж������������
	virtual int getFormatInfo();//��ȡ���ֵ�������
	virtual int stepPlayerInfo();
	virtual int getSendActions(vector<CAction>& subVec, int& RoundIndex);
	virtual int getMe(CPlayer& p);
	virtual int quitRoom(int roomID);

	//////////////////////////////////////////////////////////////////////////
	virtual string getTitleInfo();

	virtual CPlayer* getPlayerByName(string strName);
	//////////////////////////////////////////////////////////////////////////
	CEngine * pEngine;
// 	CRoom room;
// 	string userName;
// 	
// 	vector<CPlayer> vplayers;
// 	map<string ,vector<CAction> > vShowActions; //��ʾ����
// 	map<int,vector<string> > vdiscreption;//�����ţ����
// 	map<int,vector<CFormatUnit *> > vformatdescription;//�����ţ����
// 	map<int,vector<CPlayer> > vdispPlayer;
	
	
protected:

	//CClientStatusSingle * m_pCmdStatus;
};


#endif