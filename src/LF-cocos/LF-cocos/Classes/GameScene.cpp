#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "GameScene.h"
#include "HouseScene.h"
#include "cocoa/CCArray.h"
#include "textures/CCTextureCache.h"
#include "TextInputTest.h"
#include "tool.h"
//#include "model_comm.h"
#ifdef MAKE_ANDRIOD
#include "status_comm.h"
#else 
#include "LFClient/status_comm.h"
#endif
#include "cocos-ext.h"
#include "CCharBattleGirl.h"

#define TABLE_USER 1
#define TABLE_ROOM 2

using namespace cocos2d;
//char g_Control1Name[30];
//int showKeyFlag = 0;

extern CRoom g_room;
extern string g_userName;

CCScene* GameScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameScene *layer = GameScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	m_pCmdStatus = new CClientStatusGame(g_room,g_userName);
    bool bRet = false;
	isLonging = false;
	initfinished = false;
	tf1 = NULL;
	m_selectedRoomID = -1;
    do 
    {
		scheduleUpdate();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		{
			CC_BREAK_IF(! CCLayer::init());
			//初始化背景
			CCSprite* pSprite = CCSprite::create("map/Game.png");
			CC_BREAK_IF(! pSprite);

			pSprite->setPosition(ccp(size.width/2, size.height/2));
			this->addChild(pSprite, 0);
		}
		
		//////////////////////////////////////////////////////////////////////////point
		CCPoint positions[6];

		{
			CCArray * animFrames = new CCArray(20);
			//CCArray * animFrames;
			CCTexture2D * point = CCTextureCache::sharedTextureCache()->addImage("char/point.png");
			CCSpriteFrame * pontFrame[20];
			
			int perWith =192 , perHigh =192, maxcol = 5 , maxrow = 4;
			int nowRow = 0 , nowCol = 0;
			for (int i = 0 ; i < 20; ++i)
			{	
				pontFrame[i] = CCSpriteFrame::createWithTexture(point,cocos2d::CCRectMake(perWith*nowCol,perHigh*nowRow,perWith,perHigh));
				nowCol++;
				if (nowCol >= maxcol)
				{
					nowRow++;
					nowCol = 0;
				}
				animFrames->addObject(pontFrame[i]);
			}
			CCAnimation * animation = new CCAnimation();
			animation->initWithSpriteFrames(animFrames,0.1f);
			CCSprite * pointSprite[6];
			int a = 0 , b = 0;
			for (int i = 0 ; i < 6 ; ++i)
			{	
				pointSprite[i] = CCSprite::createWithSpriteFrame(pontFrame[0]);
				pointSprite[i]->setPosition(ccp(100,100));
				pointSprite[i]->setScale(0.5);
				a = 140* (( i % 3) + 1);
				b = 70* ((i/3) + 1)- 30;
				pointSprite[i]->setPosition(ccp( a ,b ));
				positions[i].setPoint(a,b);
				this->addChild(pointSprite[i]);
				CCAnimate * animate = CCAnimate::create(animation);
				pointSprite[i]->runAction(CCRepeatForever::create(animate));
			}
		}
		//////////////////////////////////////////////////////////////////////////人物
		int pos[4] = { 0,2,3,5};
		for (int i = 0 ; i < 4 ; ++i)
		{
			playerSprite[i] = new CCharBattleGirl(this,"test");
			playerSprite[i]->initMove();
			playerSprite[i]->setPosition(positions[pos[i]].x,positions[pos[i]].y+20);
			if (i % 2 == 1)
			{
				playerSprite[i]->setFlipX(true);
			}
			//this->addChild(playerSprite[i]->getSprite(),1);
			playerSprite[i]->showmove();
		}
		//////////////////////////////////////////////////////////////////////////titleinfo
		{
			pTitleLabel = new LFTextWithBack(m_pCmdStatus->getTitleInfo(),this);
			CCSize textsize = pTitleLabel->getContnetSize();
			pTitleLabel->showInPosition(5,size.height-textsize.height);
		}
		//////////////////////////////////////////////////////////////////////////update
		this->myupdate(1);
		//////////////////////////////////////////////////////////////////////////roomInfo
		{
			pTitleChat = new LFTextWithBack("聊天信息:",this);
			pTitleChat->showInPosition(5,0);
		}
		//////////////////////////////////////////////////////////////////////////TIMER
		schedule(schedule_selector(GameScene::myupdate),1.0f);  
		//////////////////////////////////////////////////////////////////////////menu
		CCMenuItemImage *pChatItem = CCMenuItemImage::create("MenuButton/liaotian.png","MenuButton/liaotian2.png",this,menu_selector(GameScene::menuChatCallback));
		pChatItem->setScale(0.6);
		CCMenuItemImage *pMoveItem = CCMenuItemImage::create("MenuButton/move.png","MenuButton/move2.png",this,menu_selector(GameScene::menuMoveCallback));
		pMoveItem->setScale(0.6);
		CCMenuItemImage *pFistItem = CCMenuItemImage::create("MenuButton/quan.png","MenuButton/quan2.png",this,menu_selector(GameScene::menuFistCallback));
		pFistItem->setScale(0.6);
		CCMenuItemImage *pQigongItem = CCMenuItemImage::create("MenuButton/qigong.png","MenuButton/qigong2.png",this,menu_selector(GameScene::menuQigongCallback));
		pQigongItem->setScale(0.6);
		CCMenuItemImage *pBishaItem = CCMenuItemImage::create("MenuButton/bisha.png","MenuButton/bisha2.png",this,menu_selector(GameScene::menuBishaCallback));
		pBishaItem->setScale(0.6);
		CCMenuItemImage *pHuanxianItem = CCMenuItemImage::create("MenuButton/huanxian.png","MenuButton/huanxian2.png",this,menu_selector(GameScene::menuHuanxianCallback));
		pHuanxianItem->setScale(0.6);
		CCMenuItemImage *pShanbiItem = CCMenuItemImage::create("MenuButton/shan.png","MenuButton/shan2.png",this,menu_selector(GameScene::menuShanbiCallback));
		pShanbiItem->setScale(0.6);
		CCMenuItemImage *pShuaiItem = CCMenuItemImage::create("MenuButton/shuai.png","MenuButton/shuai2.png",this,menu_selector(GameScene::menuShuaiCallback));
		pShuaiItem->setScale(0.6);
		CCMenu *menu = CCMenu::create(pChatItem,pMoveItem,pFistItem,pQigongItem,pBishaItem,pHuanxianItem,pShanbiItem,pShuaiItem,NULL);
		menu->setAnchorPoint(ccp(0,0));
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(200,280));
		this->addChild(menu,1);
		this->setTouchEnabled(true); 
		//////////////////////////////////////////////////////////////////////////
		initfinished = true;
        bRet = true;
    } while (0);
    return bRet;
}

void GameScene::menuChatCallback(CCObject* pSender)
{
	playerSprite[0]->showqingzhu();
}
void GameScene::menuMoveCallback(CCObject* pSender)
{
	playerSprite[0]->showmove();
}
void GameScene::menuFistCallback(CCObject* pSender)
{
	playerSprite[0]->showfist();
}
void GameScene::menuQigongCallback(CCObject* pSender)
{
	//playerSprite[0]->showqigong();
}
void GameScene::menuBishaCallback(CCObject* pSender)
{
	playerSprite[0]->showbisha();
}
void GameScene::menuHuanxianCallback(CCObject* pSender)
{
	playerSprite[0]->showdead();
}
void GameScene::menuShanbiCallback(CCObject* pSender)
{
	playerSprite[0]->showaviod();
}
void GameScene::menuShuaiCallback(CCObject* pSender)
{
	playerSprite[0]->showbeattack();
}

void GameScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{   

}

void GameScene::setCmdStatus(CClientStatusGame * pCmdStatus)
{
	m_pCmdStatus = pCmdStatus;
}

void GameScene::myupdate(float tmd)
{
	if (m_pCmdStatus->getPlayerInfo() == RTN_SUCCESS)
	{
		if (m_pCmdStatus->RoundIndex == 1000)
		{
			m_pCmdStatus->isGameOver = 1;
		}
		else
		{
			if (m_pCmdStatus->LastRoundIndex != m_pCmdStatus->RoundIndex)
			{
				//新的一轮开始了
				m_pCmdStatus->LastRoundIndex = m_pCmdStatus->RoundIndex;
				m_pCmdStatus->isActed = 0;
			}
		}
		//showPlayer(m_pCmdStatus->vplayers);
	}
	if (m_pCmdStatus->getLastActionInfo() == RTN_SUCCESS)
	{
		//showComand(m_pCmdStatus->vShowActions,m_pCmdStatus->vplayers);
	}
	if (m_pCmdStatus->getDetailInfo() == RTN_SUCCESS)
	{
		if (m_pCmdStatus->stepPlayerInfo() == RTN_SUCCESS)
		{
			//showDetail(m_pCmdStatus->vdiscreption,m_pCmdStatus->vdispPlayer,m_pCmdStatus->vShowActions,m_pCmdStatus->vplayers);
		}
	}

	if (m_pCmdStatus->getFormatInfo() == RTN_SUCCESS)
	{

	}
	return;
}
