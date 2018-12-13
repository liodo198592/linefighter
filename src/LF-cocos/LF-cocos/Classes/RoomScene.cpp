#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "RoomScene.h"
#include "HouseScene.h"
#include "GameScene.h"
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
#include "LFTextWithBack.h"
#include "TestGameScene.h"

#define TABLE_USER 1
#define TABLE_ROOM 2

using namespace cocos2d;
//char g_Control1Name[30];
//int showKeyFlag = 0;

extern CRoom g_room;
extern string g_userName;

CCScene* RoomScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        RoomScene *layer = RoomScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool RoomScene::init()
{
	m_pCmdStatus = new CClientStatusRoom(g_room,g_userName);
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
			CCSprite* pSprite = CCSprite::create("map/Room.png");
			float picwidth = pSprite->getContentSize().width;
			float picheight = pSprite->getContentSize().height;
			pSprite->setScaleX(size.width/picwidth);
			pSprite->setScaleY(size.height/picheight);
			CC_BREAK_IF(! pSprite);

			pSprite->setPosition(ccp(size.width/2, size.height/2));
			this->addChild(pSprite, 0);
		}
		
		//////////////////////////////////////////////////////////////////////////titleinfo
		{
			pTitleLabel = new LFTextWithBack(m_pCmdStatus->getTitleInfo(m_pCmdStatus->userName),this);
			CCSize textsize = pTitleLabel->getContnetSize();
			pTitleLabel->showInPosition(size.width/128,size.height-textsize.height);
		}
		//////////////////////////////////////////////////////////////////////////update
		this->myupdate(1);
		//////////////////////////////////////////////////////////////////////////userInfo
		{
			pTitleUserLabel = new LFTextWithBack("房间用户列表",this);
			CCSize textsize = pTitleUserLabel->getContnetSize();
			pTitleUserLabel->showInPosition(size.width/128,size.height-size.height/10-textsize.height);
		
			CCSize visibSize=CCDirector::sharedDirector()->getVisibleSize();  
			UserTableView=cocos2d::extension::CCTableView::create(this, CCSizeMake(size.width/1.6, size.height/3.2));  
			UserTableView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);  
			UserTableView->setAnchorPoint(ccp(0, 0)); 
			UserTableView->setPosition(0,size.height/2);
			UserTableView->setDelegate(this);
			UserTableView->setVerticalFillOrder(cocos2d::extension::kCCTableViewFillTopDown);  
			this->addChild(UserTableView,2);  
			UserTableView->reloadData();//读取数据
			UserTableView->setTag(TABLE_USER);
		}
		//////////////////////////////////////////////////////////////////////////roomInfo
		{
			pTitleChat = new LFTextWithBack("聊天信息:",this);
			CCSize textsize = pTitleChat->getContnetSize();
			pTitleChat->showInPosition(size.width/128,size.height/6-textsize.height);
		}
		//////////////////////////////////////////////////////////////////////////TIMER
		schedule(schedule_selector(RoomScene::myupdate),3.0f);  
		//////////////////////////////////////////////////////////////////////////menuButton   
		CCMenuItemImage *pReadyItem = CCMenuItemImage::create("MenuButton/ready.png","MenuButton/ready-un.png",this,menu_selector(RoomScene::menuReadyCallback));
		pReadyItem->setScale(0.6);
		CCMenuItemImage *pChatItem = CCMenuItemImage::create("MenuButton/chat.png","MenuButton/chat-un.png",this,menu_selector(RoomScene::menuChatCallback));
		pChatItem->setScale(0.6);
		CCMenuItemImage *pExitItem = CCMenuItemImage::create("MenuButton/exit.png","MenuButton/exit-un.png",this,menu_selector(RoomScene::menuExitCallback));
		pExitItem->setScale(0.6);
		CCMenu *menu = CCMenu::create(pReadyItem,pChatItem,pExitItem,NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		CCPoint Storypoint(ccp(size.width/2, size.height/10));
		menu->setPosition(Storypoint);
		this->addChild(menu,1);
		this->setTouchEnabled(true);  
		//////////////////////////////////////////////////////////////////////////
		initfinished = true;
        bRet = true;
    } while (0);
    return bRet;
}

void RoomScene::menuReadyCallback(CCObject* pSender)
{
	if (isLonging == false)
	{
		if(m_pCmdStatus->readytoStart(m_pCmdStatus->room.getRoomID()) != RTN_SUCCESS)
		{
			pTitleLabel->setString("准备失败");
		}
		else
		{
			isLonging = true;
		}
	}
}

void RoomScene::menuChatCallback(CCObject* pSender)
{
	std::string china="请输入聊天信息";     
	GBKToUTF8(china,"gbk","utf-8");    
	if (tf1 == NULL)
	{
		tf1 = CCTextFieldTTF::textFieldWithPlaceHolder(china.c_str(),"Arial",30);
		tf1->setAnchorPoint(ccp(0,0));
		tf1->setPosition(ccp(300,120));
		tf1->setDelegate(this);
		this->addChild(tf1);	
	}
	else
	{
		tf1->detachWithIME();
	}
	tf1->setString("input:");
	chatContent = "";
	//開啟文字輸入框
	tf1->attachWithIME();
}

void RoomScene::menuExitCallback(CCObject* pSender)
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = HouseScene::scene();
	//CClientStatusHouse * pStatLogin = new CClientStatusHouse(g_userName);
	//((HouseScene *)pScene)->setCmdStatus(pStatLogin);
	CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
	pDirector->replaceScene(transitionScene);
}

void RoomScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{   

}

void RoomScene::setCmdStatus(CClientStatusRoom * pCmdStatus)
{
	m_pCmdStatus = pCmdStatus;
}

void RoomScene::myupdate(float tmd)
{
	//获取User列表
	if (m_pCmdStatus->getRoomUser(m_pCmdStatus->room.getRoomID()) != RTN_SUCCESS)
	{
		CCLOG("getRoomUser error");
	}
	else
	{
		CCLOG("getRoomUser success");
	}

	if (m_pCmdStatus->getMyStatus() == CUSER_READY)
	{
		pTitleLabel->setString("请耐心等待其他人准备");
	}
	if (m_pCmdStatus->getMyStatus() == CUSER_GAMMING)
	{
		//切换到下一个场景
		CCDirector *pDirector = CCDirector::sharedDirector();
		CCScene *pScene = TestGameScene::scene();
		CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
		pDirector->replaceScene(transitionScene);
	}

	string chatinfor = m_pCmdStatus->getChatInfo();
	if (initfinished)
	{
		if (strlen(chatinfor.c_str()) > 0)
		{
			pTitleChat->setString(chatinfor.c_str());
		}
		UserTableView->reloadData();
	}
}


unsigned int RoomScene::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)  
{  
	//获取数据多少
	int num = 0;
	int tag = table->getTag();
	num = m_pCmdStatus->room.getmaxPlayer();
	return num;  
}  

cocos2d::extension::CCTableViewCell* RoomScene::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)  
{  
	float with = 100;
	int cellrate = 100;
	CCString *nameString;
	int tagindex = 0;
	int roomid = 0;
	int tag = table->getTag();
	string showStr = "";
	
	with = 400;
	showStr = m_pCmdStatus->getRoomUserInfo(idx);
	   
	GBKToUTF8(showStr,"gbk","utf-8");    
	nameString=CCString::create(showStr);  

	CCLOG("user table %d name = %s",idx,nameString->getCString());
	cocos2d::extension::CCTableViewCell *cell = table->dequeueCell();  

	if (!cell) {  
		cell = new cocos2d::extension::CCTableViewCell();   
		cell->autorelease();  
	}  
	else  
	{  
		cell->removeAllChildrenWithCleanup(true);   // 乱序在这里解决  
	}  
	CCSprite *bgSprite;
	bgSprite = CCSprite::create("MenuButton/textBackGroud.png");  
	bgSprite->setScaleX(4);
	float picwidth = bgSprite->getContentSize().width;
	float picheight = bgSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	bgSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	bgSprite->setAnchorPoint(CCPointZero);
	bgSprite->setScaleX((with)/picwidth);
	bgSprite->setScaleY((20)/picheight);
	bgSprite->setPosition(ccp(0,0));  
	bgSprite->setTag(tagindex);  
	cell->addChild(bgSprite,3);

	CCLabelTTF *pLabel = CCLabelTTF::create(nameString->getCString(), "Arial", 14);  
	CCRect rectLable(0,0,with,20);
	pLabel->setTextureRect(rectLable);
	pLabel->setTag(tagindex);
	pLabel->setColor(ccRED);
	pLabel->setAnchorPoint(CCPointZero);
	//pLabel->setPosition(CCPointZero);
	pLabel->setPosition(ccp(0, 0));  
	cell->setTag(tagindex);
	cell->addChild(pLabel,4);
	return cell;  
}  


CCSize RoomScene::cellSizeForTable(cocos2d::extension::CCTableView *table)  
{  
	CCSize visibSize=CCDirector::sharedDirector()->getVisibleSize(); 
	int tag = table->getTag();

	return CCSizeMake(400, 24);  
}  

void RoomScene::tableCellHighlight(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLOG("wwww");  
	m_selectedRoomID = cell->getTag();
	CCTexture2D *aTexturex=CCTextureCache::sharedTextureCache()->addImage("MenuButton/textBackGroud-selected.png");  
	int tag = table->getTag();
	float with = 100;
	int cellrate = 100;

	with = 400;
	cellrate = 5000;

	CCSprite *pSpritex=(CCSprite *)cell->getChildByTag(cell->getTag());  
	float picwidth = pSpritex->getContentSize().width;
	float picheight = pSpritex->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSpritex->setTexture(aTexturex);  
	pSpritex->setTextureRect(rectRoom);//设置其为宽20，高20
	pSpritex->setScaleX((with)/picwidth);
	pSpritex->setScaleY((20)/picheight);
}  

void RoomScene::tableCellUnhighlight(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLOG("dddd");  
	CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("MenuButton/textBackGroud.png");  
	int tag = table->getTag();
	float with = 100;
	int cellrate = 100;
	with = 400;
	CCSprite *pSprite=(CCSprite *)cell->getChildByTag(cell->getTag());  
	float picwidth = pSprite->getContentSize().width;
	float picheight = pSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSprite->setTexture(aTexture);  
	pSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	pSprite->setScaleX((with)/picwidth);
	pSprite->setScaleY((20)/picheight);
}  


void RoomScene::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLog("cell touched at index: %i", cell->getIdx());
	//m_selectedRoomID = cell->getTag();
	//tableCellHighlight(table,cell);
}  

void RoomScene::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
	CCLOG("scroll");
}

void RoomScene::scrollViewDidZoom(cocos2d::extension::CCScrollView* view)
{
	 CCLOG("zoom");
}

void RoomScene::scrollBar(cocos2d::extension::CCTableView* table)
{
	return;
}

bool RoomScene:: onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
	CCLOG("启动输入");
	return false;
	//    return true;//(不启动)
}
//    当用户关闭虚拟键盘的时候回调函数
bool RoomScene::  onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
	CCLOG("关闭输入");
	CChatInfo cChatInfo;
	cChatInfo.setcontent(chatContent);
	cChatInfo.setsenderNamee(m_pCmdStatus->userName);
	SDate date;
	int iNow = OSgetNowSeconds();
	TimeToDate(date,iNow);
	cChatInfo.settime(date);

	if(m_pCmdStatus->sendChartInfo(m_pCmdStatus->room.getRoomID(),cChatInfo) == RTN_SUCCESS)
	{
		string cont = "发送聊天信息成功";  
		GBKToUTF8(cont,"gbk","utf-8");    
		sender->setString(cont.c_str());
	}
	//this->removeChild(sender,true);
	return false;
}
//    当用户进行输入 虚拟键盘的时候回调函数
bool  RoomScene:: onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
	CCLOG("输入字符");
	if (*text  != 0x0d && *text != 0x0a)
	{
		chatContent += text;
	}
	tf1->setColor(ccRED);
	tf1->setPlaceHolder(chatContent.c_str());

	return false;
	//    return true;(不输入)
}
//    当用户进行删除文字 虚拟键盘的时候回调函数
bool RoomScene:: onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
	CCLOG("删除字符");
	return false;
	//    return true;(不删除)
}