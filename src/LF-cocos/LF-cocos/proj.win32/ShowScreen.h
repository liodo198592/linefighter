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
	int bigPos;//大位置1,2,3,4,5,6
	PerMapPos posDirect;//详细位置
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

	//增加动作时使用的玩家位置记录，只记录大位置
	map<string,int> m_PlayerTempPos;

	CAbstractMap* m_pMap;

	//三轮的展示动作，first表示轮次0,1,2,
	map<int,vector<CShowAction*> > m_mapShowAction;


	int m_iCueentShowRound;//当前播放的m_mapShowAction中的first，轮数
	int m_iCurrentShowIndex;//当前轮下的vector<CShowAction*> index

	int m_iShowStat;//是否在演动画，1是 0否
};

//场景模式的ShowScreen
class CShowScreenSingle : public CShowScreen
{
public:
	CShowScreenSingle(map<string,CCharBattleGirl *> vChars,CCLayer* pLayer,CAbstractMap* pMap);
	~CShowScreenSingle();

	//初始化为移动已经存在的人物至指定位置
	virtual void initPlayers(vector<CPlayer> vecPlayer);

};
#endif

