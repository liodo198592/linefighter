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
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"����˹",size.width/6.4,size.height/7));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"����˹","��ߵĻ���Ư������"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"����˹","����ô���ˡ�������ô��㻹��������"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"����˹","��������ʲô"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"���",size.width/2.56,size.height/4));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"������A",size.width/1.8,size.height/6));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"������A","���ܣ�"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Move,"������B",size.width/1.5,size.height/4));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"������B","����������ˣ������ܣ�"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"���","��������˹������ô���Ǳߣ����ܣ�����ң�"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"����˹","�����ǡ�����"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"������A","��ô����һ���ڣ�һ��ɱ�ˣ�"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"���","���ܡ��������"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_Talk,"����˹","������������"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"��һ�£�Ӫ�ȣ�"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"ʤ��Ŀ�꣺������㣡"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"ʧ��Ŀ�꣺�Լ�������"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"׼����"));
	vTalkString[0].push_back(SStoryTalkInfo(EShowType_TitleText,"��ʼ��"));

	vChars["����˹"] = new CCharBattleGirl(this,"����˹",DIRECTION_RIGHT,MAX_HP);
	//vChars["����˹"]->initMove();
	vChars["����˹"]->setPosition(-size.width/10,size.height/7);
	vChars["����˹"]->showmove();
	pTalkTextChris["����˹"] = new LFTalkText("char/talkframechris.png","char/ChrisPhoto.png",this);
	pTalkTextChris["����˹"]->setScale(0);
	
	vChars["���"] = new CCharBattleGirl(this,"���",DIRECTION_LEFT,MAX_HP/2);
	//vChars["���"]->initMove();
	vChars["���"]->setBodyColor(ccGREEN);
	vChars["���"]->setPosition(size.width + size.width/10,size.height/4);
	vChars["���"]->showmove();
	//vChars["���"]->setFlipX(true);
	pTalkTextChris["���"] = new LFTalkText("char/talkFrame.png","char/ChrisPhoto.png",this);
	pTalkTextChris["���"]->setScale(0);
	pTalkTextChris["���"]->pSpritePhoto->setColor(ccGREEN);

	
	vChars["������A"] = new CCharBattleGirl(this,"������A",DIRECTION_LEFT,MAX_HP);
	//vChars["������A"]->initMove();
	vChars["������A"]->setBodyColor(ccBLACK);
	vChars["������A"]->setPosition(size.width + size.width/10,size.height/6);
	vChars["������A"]->showmove();
	//vChars["������A"]->setFlipX(true);
	pTalkTextChris["������A"] = new LFTalkText("char/talkFrame.png","char/ChrisPhoto.png",this);
	pTalkTextChris["������A"]->setScale(0);
	pTalkTextChris["������A"]->pSpritePhoto->setColor(ccBLACK);

	vChars["������B"] = new CCharBattleGirl(this,"������B",DIRECTION_LEFT,MAX_HP);
	//vChars["������B"]->initMove();
	vChars["������B"]->setBodyColor(ccBLACK);
	vChars["������B"]->setPosition(size.width + size.width/10,size.height/4);
	vChars["������B"]->showmove();
	//vChars["������B"]->setFlipX(true);
	pTalkTextChris["������B"] = new LFTalkText("char/talkFrame.png","char/ChrisPhoto.png",this);
	pTalkTextChris["������B"]->setScale(0);
	pTalkTextChris["������B"]->pSpritePhoto->setColor(ccBLACK);

	talkindex = -1;

	return true;
}

// on "init" you need to initialize your instance
bool ChrisScene1::init()
{
	engin = new CEngine();
	CRoom room;
	room.sethostUserName("����˹");
	room.setmaxPlayer(4);
	room.setnowPlayer(1);
	room.setRoomID(1);
	room.setRoomName("�����һ��");
	room.setRoomStatus(EROOM_STATUS_GAMING);
	m_pCmdStatus = new CClientStatusSingle(engin,room,"����˹");
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
			//��ʼ������
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
			pTitleLabel = new LFTextWithBack("��һ�£�Ӫ��",this);
			CCSize textsize = pTitleLabel->getContnetSize();
			pTitleLabel->showInPosition(size.width/128,size.height-textsize.height);
		}		
		//////////////////////////////////////////////////////////////////////////��ʼ��ť
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
	//	if (strcmp(it->first.c_str(),"����˹")==0)
	//	{
	//		p.setPosition(1,1);
	//		p.setTeam(1);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	if (strcmp(it->first.c_str(),"���")==0)
	//	{
	//		p.setPosition(1,2);
	//		p.setTeam(1);
	//		CFLAI ai;
	//		p.setAI(ai);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	if (strcmp(it->first.c_str(),"������A")==0)
	//	{
	//		p.setPosition(2,1);
	//		p.setTeam(2);
	//		CFLAI ai;
	//		p.setAI(ai);
	//		p.setHp(it->second->getCurrentHp());
	//	}
	//	if (strcmp(it->first.c_str(),"������B")==0)
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
				if (strcmp(it->first.c_str(),"����˹")==0)
			  {
				  p.setPosition(1,1);
				  p.setTeam(1);
				  p.setHp(it->second->getCurrentHp());
			  }
			  if (strcmp(it->first.c_str(),"���")==0)
			  {
				  p.setPosition(1,2);
				  p.setTeam(1);
				  CFLAI ai;
				  p.setAI(ai);
				  p.setHp(it->second->getCurrentHp());
			  }
			  if (strcmp(it->first.c_str(),"������A")==0)
			  {
				  p.setPosition(2,1);
				  p.setTeam(2);
				  CFLAI ai;
				  p.setAI(ai);
				  p.setHp(it->second->getCurrentHp());
			  }
			  if (strcmp(it->first.c_str(),"������B")==0)
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