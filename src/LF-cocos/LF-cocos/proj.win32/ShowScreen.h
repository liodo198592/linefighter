#ifndef _SHOW_SCREEN_H_
#define _SHOW_SCREEN_H_
#include "cocos2d.h"
#include "global_cocos.h"
#include "CCharBattleGirl.h"
#include "MapGround.h"
#include "CPlayer.h"
#ifdef MAKE_ANDRIOD
#include "CClientStatusGame.h"
#include "ClientStatusGameBase.h"
#else
#include "LFClient\CClientStatusGame.h"
#include "LFClient\ClientStatusGameBase.h"
#endif // MAKE_ANDRIOD



#include "CAction.h"
#include "ShowAction.h"
USING_NS_CC;

struct SPlayerPos
{
	int bigPos;//��λ��1,2,3,4,5,6
	PerMapPos posDirect;//��ϸλ��
};



class CShowScreen
{
public:
	CShowScreen(CCLayer* pLayer,CAbstractMap* pMap);
	~CShowScreen(void);

	virtual void initPlayers(vector<CPlayer> vecPlayer);

	int getMyPos();

	void showGaming(CClientStatusGameBase* pCmdStatus);

	bool showingAction();

	void showAction();
protected:
	PerMapPos getMapPosByIndex(int iIndex);
	CCPDirection getDirectionByPosIndex(int iIndex);

	void initPlayerStat(string PlayerName,CCPoint showPos,int iHp);

	ShowActionType getShowTypeByServerType(EACTION serverType);

	void generateShowAction(CClientStatusGameBase* pCmdStatus);


	void updatePlayerHp(string strPlayer);

	void addShowAction(vector<CShowAction*>& vecShowAction,CShowAction* pShowAction);
protected:
	CCLayer* m_pLayer;

	map<string,CCharBattleGirl*> m_pBattlePlayer;
	map<string,SPlayerPos> m_pPlayerPos;
	map<string,CCLabelTTF*> m_mapPlayerStatShow;

	//���Ӷ���ʱʹ�õ����λ�ü�¼��ֻ��¼��λ��
	map<string,int> m_PlayerTempPos;

	CAbstractMap* m_pMap;

	//���ֵ�չʾ������first��ʾ�ִ�0,1,2,
	map<int,vector<CShowAction*> > m_mapShowAction;


	int m_iCueentShowRound;//��ǰ���ŵ�m_mapShowAction�е�first������
	int m_iCurrentShowIndex;//��ǰ���µ�vector<CShowAction*> index

	int m_iShowStat;//�Ƿ����ݶ�����1�� 0��
};

//����ģʽ��ShowScreen
class CShowScreenSingle : public CShowScreen
{
public:
	CShowScreenSingle(map<string,CCharBattleGirl *> vChars,CCLayer* pLayer,CAbstractMap* pMap);
	~CShowScreenSingle();

	//��ʼ��Ϊ�ƶ��Ѿ����ڵ�������ָ��λ��
	virtual void initPlayers(vector<CPlayer> vecPlayer);

};
#endif

