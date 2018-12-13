#include "TestGameScene.h"
#ifdef MAKE_ANDRIOD
#include "status_comm.h"
#else 
#include "LFClient/status_comm.h"
#endif

extern CRoom g_room;
extern string g_userName;

#define LEFT_BUTTON_QG	10
#define RIGHT_BUTTON_QG	11
#define LEFT_BUTTON_BS	12
#define RIGHT_BUTTON_BS	13
#define POS1_BUTTON	21
#define POS2_BUTTON	22
#define POS3_BUTTON	23

TestGameScene::TestGameScene(void)
{
}


TestGameScene::~TestGameScene(void)
{
}


CCScene* TestGameScene::scene()
{
	CCScene* pScene = CCScene::create();

	TestGameScene* pLayer = TestGameScene::create();

	pScene->addChild(pLayer);

	return pScene;
}

bool TestGameScene::init()
{
	m_pCmdStatus = new CClientStatusGame(g_room,g_userName);
	CAbstractMap* pMap = new CMapGround;
	pMap->initCoordinate();

	m_pShowScreen = new CShowScreen(this,pMap);



	//scheduleUpdate();

	//初始化背景
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	{
		//CC_BREAK_IF(! CCLayer::init());
		//初始化背景
		CCSprite* pSprite = CCSprite::create(pMap->getMapFilePath().c_str());
		//CC_BREAK_IF(! pSprite);
		float textureWide = pSprite->getTexture()->getPixelsWide();
		float textureHigh = pSprite->getTexture()->getPixelsHigh();

		//设置X,Y轴缩放比例使图片和屏幕一样大小
		pSprite->setScaleX(size.width/textureWide);
		pSprite->setScaleY(size.height/textureHigh);

		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite, 0);
	}

	schedule(schedule_selector(TestGameScene::myupdate),0.1f); 
	initButton();

// 	//测试场景模式screen
// 	map<string,CCharBattleGirl *> vChars;
// 	vChars["cyt"] = new CCharBattleGirl(this,"cyt");
// 	vChars["cyt"]->setPosition(200,200);
// 	vChars["$bot-1$"] = new CCharBattleGirl(this,"$bot-1$");
// 	vChars["$bot-1$"]->setPosition(100,100);
// 	vChars["$bot-2$"] = new CCharBattleGirl(this,"$bot-2$");
// 	vChars["$bot-2$"]->setPosition(100,100);
// 	vChars["$bot-3$"] = new CCharBattleGirl(this,"$bot-3$");
// 	vChars["$bot-3$"]->setPosition(100,100);
// 	m_pShowScreen = new CShowScreenSingle(vChars,this,pMap);


// 	CCharBattleGirl* pgirl = new CCharBattleGirl(this,"xxx",DIRECTION_RIGHT);
// 
// 	pgirl->setPosition(100,100);
// 
// 	pgirl->moveToPos(200,200);

	this->setTouchEnabled(true); 

	m_PStateHandler = new CStateHandler(m_pCmdStatus,m_pShowScreen,m_pMenuControl);

	return true;
}
void TestGameScene::initButton()
{

	m_pMenuControl = CMenuControler::create();
	this->addChild(m_pMenuControl);

}
void TestGameScene::setCmdStatus(CClientStatusGame * pCmdStatus)
{
	m_pCmdStatus = pCmdStatus;
}

void TestGameScene::myupdate(float tmd)
{
	m_PStateHandler->handleState(tmd);
	return;
}



