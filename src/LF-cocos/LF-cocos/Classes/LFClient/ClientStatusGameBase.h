#ifndef _CLIENT_STATUS_GAME_BASE_H_
#define  _CLIENT_STATUS_GAME_BASE_H_

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
#include "CClientStatus.h"
#endif // MAKE_ANDRIOD
#include "CAction.h"

class CFormatUnit;

class CClientStatusGameBase : public CCmdStatus
{
public:
	CClientStatusGameBase(void);
	~CClientStatusGameBase(void);

	virtual int getPlayerInfo() = 0;
	virtual int getLastActionInfo() = 0;//��ȡ����������ҵĶ���
	virtual int getDetailInfo() = 0;//��ȡ�������ж������������
	virtual int getFormatInfo() = 0;//��ȡ���ֵ�������
	virtual int stepPlayerInfo() = 0;
	virtual int getSendActions(vector<CAction>& subVec, int& RoundIndex) = 0;
	virtual int getMe(CPlayer& p) = 0;
	virtual int quitRoom(int roomID) = 0;

	virtual string getTitleInfo() = 0;

	virtual CPlayer* getPlayerByName(string strName) = 0;

	CRoom room;
	string userName;

	vector<CPlayer> vplayers;
	map<string ,vector<CAction> > vShowActions; //��ʾ����
	map<int,vector<string> > vdiscreption;//�����ţ����
	map<int,vector<CFormatUnit *> > vformatdescription;//�����ţ����
	map<int,vector<CPlayer> > vdispPlayer;

	int isActed;
	int isGameOver;
	int RoundIndex;
	int LastRoundIndex;

};

#endif