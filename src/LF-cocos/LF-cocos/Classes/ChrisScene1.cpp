#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "ChrisScene1.h"
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
#include "LogInScene.h"
#include "RoomScene.h"
#include "LFTextWithBack.h"


using namespace cocos2d;

CCScene* ChrisScene1::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        ChrisScene1 *layer = ChrisScene1::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool ChrisScene1::storyinit()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"克里斯",size.width/6.4,size.height/7));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"克里斯","这边的花好漂亮啊。"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"克里斯","天这么晚了。。。怎么姐姐还不回来？"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"克里斯","恩？那是什么"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"姐姐",size.width/2.56,size.height/4));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"黑衣人A",size.width/1.8,size.height/6));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"黑衣人A","别跑！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"黑衣人B",size.width/1.5,size.height/4));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"黑衣人B","你今天死定了，别想跑！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"姐姐","啊，克里斯，你怎么在那边，快跑，别管我！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"克里斯","他们是。。。"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"黑衣人A","怎么还有一个在，一起杀了！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"姐姐","快跑。。别管我"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"克里斯","不。。我来了"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"第一章：营救！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"胜利目标：保护姐姐！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"失败目标：自己死亡！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"准备！"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"开始！"));

	vChars["克里斯"] = new CCharBattleGirl(this,"克里斯",DIRECTION_RIGHT,MAX_HP);
	//vChars["克里斯"]->initMove();
	vChars["克里斯"]->setPosition(-size.width/10,size.height/7);
	vChars["克里斯"]->showmove();
	pTalkTextChris["克里斯"] = new LFTalkText("char/talkframechris.png","char/ChrisPhoto.png",this);
	pTalkTextChris["克里斯"]->setScale(0);
	
	vChars["姐姐"] = new CCharBattleGirl(this,"姐姐",DIRECTION_LEFT,MAX_HP/2);
	//vChars["姐姐"]->initMove();
	vChars["姐姐"]->setBodyColor(ccGREEN);
	vChars["姐姐"]->setPosition(size.width + size.width/10,size.height/4);
	vChars["姐姐"]->showmove();
	//vChars["姐姐"]->setFlipX(true);
	pTalkTextChris["姐姐"] = new LFTalkText("char/talkFrame.png","char/ChrisPhoto.png",this);
	pTalkTextChris["姐姐"]->setScale(0);
	pTalkTextChris["姐姐"]->pSpritePhoto->setColor(ccGREEN);

	
	vChars["黑衣人A"] = new CCharBattleGirl(this,"黑衣人A",DIRECTION_LEFT,MAX_HP);
	//vChars["黑衣人A"]->initMove();
	vChars["黑衣人A"]->setBodyColor(ccBLACK);
	vChars["黑衣人A"]->setPosition(size.width + size.width/10,size.height/6);
	vChars["黑衣人A"]->showmove();
	//vChars["黑衣人A"]->setFlipX(true);
	pTalkTextChris["黑衣人A"] = new LFTalkText("char/talkFrame.png","char/ChrisPhoto.png",this);
	pTalkTextChris["黑衣人A"]->setScale(0);
	pTalkTextChris["黑衣人A"]->pSpritePhoto->setColor(ccBLACK);

	vChars["黑衣人B"] = new CCharBattleGirl(this,"黑衣人B",DIRECTION_LEFT,MAX_HP);
	//vChars["黑衣人B"]->initMove();
	vChars["黑衣人B"]->setBodyColor(ccBLACK);
	vChars["黑衣人B"]->setPosition(size.width + size.width/10,size.height/4);
	vChars["黑衣人B"]->showmove();
	//vChars["黑衣人B"]->setFlipX(true);
	pTalkTextChris["黑衣人B"] = new LFTalkText("char/talkFrame.png","char/ChrisPhoto.png",this);
	pTalkTextChris["黑衣人B"]->setScale(0);
	pTalkTextChris["黑衣人B"]->pSpritePhoto->setColor(ccBLACK);

	talkindex = -1;

	return true;
}

// on "init" you need to initialize your instance
bool ChrisScene1::init()
{
	engin = new CEngine();
	CRoom room;
	room.sethostUserName("克里斯");
	room.setmaxPlayer(4);
	room.setnowPlayer(1);
	room.setRoomID(1);
	room.setRoomName("剧情第一关");
	room.setRoomStatus(EROOM_STATUS_GAMING);
	m_pCmdStatus = new CClientStatusSingle(engin,room,"克里斯");
	gamestart = 0;
    bool bRet = false;
	isLonging = false;
	initfinished = false;
	gotoRoom = false;
	m_selectedRoomID = -1;
    do 
    {
		scheduleUpdate();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		{
			CC_BREAK_IF(! CCLayer::init());
			//初始化背景
			CCSprite* pSprite = CCSprite::create("map/ChrisScene1.png");
			float picwidth = pSprite->getContentSize().width;
			float picheight = pSprite->getContentSize().height;
			pSprite->setScaleX(size.width/picwidth);
			pSprite->setScaleY(size.height/picheight);
			CC_BREAK_IF(! pSprite);

			pSprite->setPosition(ccp(size.width/2, size.height/2));
			this->addChild(pSprite, 0);
		}

		{
			pTitleLabel = new LFTextWithBack("第一章：营救",this);
			CCSize textsize = pTitleLabel->getContnetSize();
			pTitleLabel->showInPosition(size.width/128,size.height-textsize.height);
		}		
		//////////////////////////////////////////////////////////////////////////开始按钮
		storyinit();
		//////////////////////////////////////////////////////////////////////////
		schedule(schedule_selector(ChrisScene1::myupdate),3.0f);  
		initfinished = true;
        bRet = true;

		titleLable = CCLabelTTF::create(" ", "Arial", 60);
		titleLable->setPosition(ccp(size.width / 2, size.height + size.height/6));
		titleLable->setColor(ccYELLOW);
		this->addChild(titleLable, 1);
		

		this->setTouchEnabled(true);  
		
    } while (0);

	m_finishInitBattle = false;
    return bRet;
}

void ChrisScene1::menuStartCallback(CCObject* pSender)
{

}

void ChrisScene1::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{   
	for (map<string,LFTalkText *>::iterator it = pTalkTextChris.begin(); it != pTalkTextChris.end(); ++it)
	{
		if (it->second->isopen())
		{
			it->second->close();
		}
	}
}


void ChrisScene1::myupdate(float tmd)
{

	vector<CPlayer> vplayers_x;

	//for (map<string,CCharBattleGirl *>::iterator it = vChars.begin(); it != vChars.end(); ++it)
	//{
	//	CPlayer p(it->first);
	//	if (strcmp(it->first.c_str(),"克里斯")==0)
	//	{
	//		p.setPosition(1,1);
	//		p.setTeam(1);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	if (strcmp(it->first.c_str(),"姐姐")==0)
	//	{
	//		p.setPosition(1,2);
	//		p.setTeam(1);
	//		CFLAI ai;
	//		p.setAI(ai);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	if (strcmp(it->first.c_str(),"黑衣人A")==0)
	//	{
	//		p.setPosition(2,1);
	//		p.setTeam(2);
	//		CFLAI ai;
	//		p.setAI(ai);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	if (strcmp(it->first.c_str(),"黑衣人B")==0)
	//	{
	//		p.setPosition(2,2);
	//		p.setTeam(2);
	//		CFLAI ai;
	//		p.setAI(ai);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	vplayers_x.push_back(p);
	//}
	//engin->setPlayers(vplayers_x);
	//pthread_create(&pidrun,NULL,runEngineSvc,(void *)engin);
	//CCLOG("pthread_create SUCCESS");

	//initBattleScene();
	////m_finishInitBattle = true;
	//schedule(schedule_selector(ChrisScene1::handleStatUpdate),0.1); 
	//unschedule(schedule_selector(ChrisScene1::myupdate));

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	bool allclosed = true;
	for (map<string,LFTalkText *>::iterator it = pTalkTextChris.begin(); it != pTalkTextChris.end(); ++it)
	{
		if (it->second->isopen())
		{
			allclosed = false;
			break;
		}
	}


	if (allclosed)
	{
		talkindex++;
		if (talkindex < (vTalkString[0]).size())
		{
			SStoryTalkInfo unit = (vTalkString[0])[talkindex];
			if (unit.eType == EShowType_Talk)
			{
				string name = unit.name;
				string info = unit.info;
				if (pTalkTextChris.find(name) != pTalkTextChris.end() && vChars.find(name) != vChars.end())
				{

					LFTalkText * p = pTalkTextChris[name];
					CCharBattleGirl * girl = vChars[name];
					float x = girl->getPositionX();
					float y = girl->getPositionY();
					p->setScale(0);
					p->setString(info);
					p->showInPosition(x+girl->getContentSizeX()/2,y+girl->getContentSizeY()/4);
					p->open();
				}
			}
			if (unit.eType == EShowType_Move)
			{
				string name = unit.name;
				int x = unit.destX;
				int y = unit.destY;
				CCharBattleGirl * girl = vChars[name];
				girl->moveToPos(2,x,y);
			}

			if (unit.eType == EShowType_TitleText)
			{
				titleLable->setPosition(ccp(size.width / 2, size.height + size.height/6));
				string info = unit.info;
				GBKToUTF8(info,"gbk","utf-8");    
				titleLable->setString(info.c_str());
				CCFiniteTimeAction *actionMove = CCMoveTo::create(1.5,ccp(size.width/2,size.height/2));
				titleLable->runAction(actionMove);  
			}
		}
		else
		{
			titleLable->setPosition(ccp(size.width / 2, size.height + size.height/6));
			vector<CPlayer> vplayers_x;
			
			for (map<string,CCharBattleGirl *>::iterator it = vChars.begin(); it != vChars.end(); ++it)
			{
				CPlayer p(it->first);
				if (strcmp(it->first.c_str(),"克里斯")==0)
			  {
				  p.setPosition(1,1);
				  p.setTeam(1);
				  p.setHp(it->second->getCurrentHp());
			  }
			  if (strcmp(it->first.c_str(),"姐姐")==0)
			  {
				  p.setPosition(1,2);
				  p.setTeam(1);
				  CFLAI ai;
				  p.setAI(ai);
				  p.setHp(it->second->getCurrentHp());
			  }
			  if (strcmp(it->first.c_str(),"黑衣人A")==0)
			  {
				  p.setPosition(2,1);
				  p.setTeam(2);
				  CFLAI ai;
				  p.setAI(ai);
				  p.setHp(it->second->getCurrentHp());
			  }
			  if (strcmp(it->first.c_str(),"黑衣人B")==0)
			  {
				  p.setPosition(2,2);
				  p.setTeam(2);
				  CFLAI ai;
				  p.setAI(ai);
				  p.setHp(it->second->getCurrentHp());
			  }
				vplayers_x.push_back(p);
			}
			engin->setPlayers(vplayers_x);
			pthread_create(&pidrun,NULL,runEngineSvc,(void *)engin);
			CCLOG("pthread_create SUCCESS");

			initBattleScene();
			//m_finishInitBattle = true;
			schedule(schedule_selector(ChrisScene1::handleStatUpdate),0.1); 
			unschedule(schedule_selector(ChrisScene1::myupdate));
		}
	}
}
void ChrisScene1::handleStatUpdate(float tmd)
{
		m_PStateHandler->handleState(tmd);
}
void * ChrisScene1::runEngineSvc(void * p)
{
	CEngine * pEngine = (CEngine *)p;
	pEngine->svc();
	return NULL;
}
void ChrisScene1::initBattleScene()
{
	CAbstractMap* pMap = new CMapGround;
	pMap->initCoordinate();

	m_pMenuControl = CMenuControler::create();
	this->addChild(m_pMenuControl);

	m_pShowScreen = new CShowScreenSingle(vChars,this,pMap);

	m_PStateHandler = new CStateHandler(m_pCmdStatus,m_pShowScreen,m_pMenuControl);

}