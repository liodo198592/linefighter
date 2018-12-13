#ifndef _TEST_GAME_SCENE_H_
#define _TEST_GAME_SCENE_H_
#include "cocos2d.h"
#include "CCharBattleGirl.h"
#include "MapGround.h"
#include "ShowScreen.h"
#ifdef MAKE_ANDRIOD
#include "status_comm.h"
#include "ClientStatusGameBase.h"
#else
#include "LFClient/status_comm.h"
#include "LFClient/ClientStatusGameBase.h"
#endif // DEBUG



#include "MenuControler.h"

USING_NS_CC;

#include "StateHandler.h"

class TestGameScene : public CCLayer
{
public:
	TestGameScene(void);
	~TestGameScene(void);

	virtual bool init();

	static CCScene* scene();

	void setCmdStatus(CClientStatusGame * pCmdStatus);
	void setUserName(string userName_x);

private:
	void initButton();


	virtual void myupdate(float tmd);




	//void changeStat(GameStat toStat);

	CREATE_FUNC(TestGameScene);


private:

	CMenuControler* m_pMenuControl;

	CCharBattleGirl* m_pBattleGirl;

	//CAbstractMap* m_pMap;

	CShowScreen* m_pShowScreen;

	CClientStatusGameBase* m_pCmdStatus;

	CStateHandler* m_PStateHandler;
	//vector<CAction> m_vecAction;
};

#endif
