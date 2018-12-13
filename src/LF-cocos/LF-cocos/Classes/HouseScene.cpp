#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
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
#include "LogInScene.h"
#include "RoomScene.h"
#include "LFTextWithBack.h"

#define TABLE_USER 1
#define TABLE_ROOM 2

using namespace cocos2d;
//char g_Control1Name[30];
//int showKeyFlag = 0;

CRoom g_room;
extern string g_userName;

CCScene* HouseScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HouseScene *layer = HouseScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HouseScene::init()
{
	m_pCmdStatus = new CClientStatusHouse(g_userName);
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
			CCSprite* pSprite = CCSprite::create("map/House.png");
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
			pTitleLabel = new LFTextWithBack(m_pCmdStatus->getTitleInfo(),this);
			CCSize textsize = pTitleLabel->getContnetSize();
			pTitleLabel->showInPosition(size.width/128,size.height-textsize.height);
		}
		//////////////////////////////////////////////////////////////////////////update
			//////////////////////////////////////////////////////////////////////////timer
		this->myupdate(1);
		//////////////////////////////////////////////////////////////////////////userInfo
		{
			pTitleUserLabel = new LFTextWithBack(m_pCmdStatus->getUserInfoStr(),this);
			CCSize textsize = pTitleUserLabel->getContnetSize();
			pTitleUserLabel->showInPosition(size.width/128,size.height-size.height/10-textsize.height);
			
			CCSize visibSize=CCDirector::sharedDirector()->getVisibleSize();  
			UserTableView=cocos2d::extension::CCTableView::create(this, CCSizeMake(size.width/5.3, size.height/1.7));  
			UserTableView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);  
			UserTableView->setAnchorPoint(ccp(0, 0)); 
			UserTableView->setPosition(0,size.height/4);
			UserTableView->setDelegate(this);
			UserTableView->setVerticalFillOrder(cocos2d::extension::kCCTableViewFillTopDown);  
			this->addChild(UserTableView,2);  
			UserTableView->reloadData();//读取数据
			UserTableView->setTag(TABLE_USER);
		}
		//////////////////////////////////////////////////////////////////////////roomInfo
		{
			pTitleRoomLabel = new LFTextWithBack(m_pCmdStatus->getRoomTotalInfoStr(),this);
			CCSize textsize = pTitleRoomLabel->getContnetSize();
			pTitleRoomLabel->showInPosition(size.width/2.1,size.height-size.height/10-textsize.height);
			
			CCSize visibSize=CCDirector::sharedDirector()->getVisibleSize();  
			UserRoomView=cocos2d::extension::CCTableView::create(this, CCSizeMake(size.width/1.3, size.height/1.7));  
			UserRoomView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical); 
			UserRoomView->setAnchorPoint(ccp(0, 0)); 
			UserRoomView->setPosition(size.width/4.3,size.height/4);
			UserRoomView->setDelegate(this);
			UserRoomView->setVerticalFillOrder(cocos2d::extension::kCCTableViewFillTopDown);  
			this->addChild(UserRoomView,2);  
			UserRoomView->reloadData();//读取数据
			UserRoomView->setTag(TABLE_ROOM);
		}
		//////////////////////////////////////////////////////////////////////////TIMER
		schedule(schedule_selector(HouseScene::myupdate),3.0f);  
		//////////////////////////////////////////////////////////////////////////menuButton   
		CCMenuItemImage *pCreateItem = CCMenuItemImage::create("MenuButton/create.png","MenuButton/create-un.png",this,menu_selector(HouseScene::menuCreateCallback));
		pCreateItem->setScale(0.6);
		CCMenuItemImage *pJionItem = CCMenuItemImage::create("MenuButton/jion.png","MenuButton/jion-un.png",this,menu_selector(HouseScene::menuJionCallback));
		pJionItem->setScale(0.6);
		CCMenuItemImage *pExitItem = CCMenuItemImage::create("MenuButton/exit.png","MenuButton/exit-un.png",this,menu_selector(HouseScene::menuExitCallback));
		pExitItem->setScale(0.6);
		CCMenu *menu = CCMenu::create(pCreateItem,pJionItem,pExitItem,NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		CCPoint Storypoint(ccp(size.width/2,size.height/5));
		menu->setPosition(Storypoint);
		this->addChild(menu,1);
		this->setTouchEnabled(true);  
		//////////////////////////////////////////////////////////////////////////
		initfinished = true;
        bRet = true;
    } while (0);
    return bRet;
}

void HouseScene::menuCreateCallback(CCObject* pSender)
{
	std::string china="输入房间名称";  
	GBKToUTF8(china,"gbk","utf-8");    
	tf1 = CCTextFieldTTF::textFieldWithPlaceHolder(china.c_str(),"Arial",30);
	tf1->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(tf1);
	tf1->setDelegate(this);
	//開啟文字輸入框
	tf1->attachWithIME();
}

void HouseScene::menuJionCallback(CCObject* pSender)
{
	CRoom room;
	if (m_selectedRoomID == -1 || isLonging == true)
	{
		return;
	}
	else
	{
		isLonging = true;
		if(m_pCmdStatus->jionRoom(room,m_selectedRoomID) == RTN_SUCCESS)
		{
			pTitleLabel->setString("success");
			CCDirector *pDirector = CCDirector::sharedDirector();
			g_room = room;
			CCScene *pScene = RoomScene::scene();
			CClientStatusRoom * pStatRoom = new CClientStatusRoom(room,m_pCmdStatus->name);
			((RoomScene *)pScene)->setCmdStatus(pStatRoom);
			CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
			pDirector->replaceScene(transitionScene);
			return ;
		}
		else
		{
			pTitleLabel->setString("error");
		}
	}
	
}

void HouseScene::menuExitCallback(CCObject* pSender)
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = LogInScene::scene();
	CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
	pDirector->replaceScene(transitionScene);
}


void HouseScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{   
}

void HouseScene::setCmdStatus(CClientStatusHouse * pCmdStatus)
{
	m_pCmdStatus = pCmdStatus;
}

void HouseScene::myupdate(float tmd)
{
	if (gotoRoom == true)
	{
		CCDirector *pDirector = CCDirector::sharedDirector();
		CCScene *pScene = RoomScene::scene();
		CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
		pDirector->replaceScene(transitionScene);
	}

	if (m_pCmdStatus->getOnlneList() != RTN_SUCCESS)
	{
		CCLOG("GET getOnlneList failed\n");
	}
	else
	{
		CCLOG("GET getOnlneList success\n");
	}
	//获取房间列表
	if (m_pCmdStatus->getRoomList() != RTN_SUCCESS)
	{
		CCLOG("GET getRoomList failed\n");
	}
	else
	{
		CCLOG("GET getRoomList success\n");
	}
	if (initfinished)
	{
		pTitleUserLabel->setString(m_pCmdStatus->getUserInfoStr());
		pTitleRoomLabel->setString(m_pCmdStatus->getRoomTotalInfoStr());
		UserTableView->reloadData();
		UserRoomView->reloadData();//读取数据
	}

	
}


unsigned int HouseScene::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)  
{  
	//获取数据多少
	int num = 0;
	int tag = table->getTag();
	if (tag == TABLE_USER)
	{
		num = m_pCmdStatus->m_UserList.size();
	}
	else if (tag == TABLE_ROOM)
	{
		num = m_pCmdStatus->m_roomList.size();
	}	
	return num;  
}  

cocos2d::extension::CCTableViewCell* HouseScene::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)  
{  
	float with = 100;
	int cellrate = 100;
	CCString *nameString;
	int tagindex = 0;
	int roomid = 0;
	int tag = table->getTag();
	string showStr = "";
	if (tag == TABLE_USER)
	{
		with = 120;
		cellrate = 100;
		tagindex = idx * cellrate +1;
		showStr = m_pCmdStatus->getOneUserStr(idx);
	}
	else if (tag == TABLE_ROOM)
	{
		with = 480;
		cellrate = 5000;
		showStr = m_pCmdStatus->getRoomInfoStr(idx,roomid);
		tagindex = roomid;
	}  
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
	CCRect rectCell(0,0,with,20);
	
	if (tagindex == m_selectedRoomID)
	{
		bgSprite = CCSprite::create("MenuButton/textBackGroud-selected.png");  
		bgSprite->setScaleX(4);
	}
	else
	{
		bgSprite = CCSprite::create("MenuButton/textBackGroud.png");  
	}
	float picwidth = bgSprite->getContentSize().width;
	float picheight = bgSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	bgSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	bgSprite->setScaleX((with)/picwidth);
	bgSprite->setScaleY((20)/picheight);
	bgSprite->setAnchorPoint(CCPointZero);  
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


CCSize HouseScene::cellSizeForTable(cocos2d::extension::CCTableView *table)  
{  
	CCSize visibSize=CCDirector::sharedDirector()->getVisibleSize(); 
	int tag = table->getTag();
	if (tag == TABLE_USER)
	{
		return CCSizeMake(120, 24);  
	}
	else if (tag == TABLE_ROOM)
	{
		return CCSizeMake(480, 24);  
	}
	return CCSizeMake(480, 24);
}  

void HouseScene::tableCellHighlight(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLOG("wwww");  
	m_selectedRoomID = cell->getTag();
	CCTexture2D *aTexturex=CCTextureCache::sharedTextureCache()->addImage("MenuButton/textBackGroud-selected.png");  
	int tag = table->getTag();
	float with = 100;
	int cellrate = 100;
	if (tag == TABLE_USER)
	{
		with = 120;
		cellrate = 100;
	}
	else if (tag == TABLE_ROOM)
	{
		with = 480;
		cellrate = 5000;
	}
	
	CCSprite *pSpritex=(CCSprite *)cell->getChildByTag(cell->getTag());  
	float picwidth = pSpritex->getContentSize().width;
	float picheight = pSpritex->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSpritex->setTexture(aTexturex);  
	pSpritex->setTextureRect(rectRoom);//设置其为宽20，高20
	pSpritex->setScaleX((with)/picwidth);
	pSpritex->setScaleY((20)/picheight);
}  

void HouseScene::tableCellUnhighlight(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLOG("dddd");  
	CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("MenuButton/textBackGroud.png");  
	int tag = table->getTag();
	float with = 100;
	int cellrate = 100;
	if (tag == TABLE_USER)
	{
		with = 120;
		cellrate = 100;
	}
	else if (tag == TABLE_ROOM)
	{
		with = 480;
		cellrate = 5000;
	}
	CCSprite *pSprite=(CCSprite *)cell->getChildByTag(cell->getTag());  
	float picwidth = pSprite->getContentSize().width;
	float picheight = pSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSprite->setTexture(aTexture);  
	pSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	pSprite->setScaleX((with)/picwidth);
	pSprite->setScaleY((20)/picheight);
}  


void HouseScene::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLog("cell touched at index: %i", cell->getIdx());
	//m_selectedRoomID = cell->getTag();
	//tableCellHighlight(table,cell);
}  

void HouseScene::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
	CCLOG("scroll");
}

void HouseScene::scrollViewDidZoom(cocos2d::extension::CCScrollView* view)
{
	 CCLOG("zoom");
}

void HouseScene::scrollBar(cocos2d::extension::CCTableView* table)
{
	return;
}

bool HouseScene:: onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
	CCLOG("启动输入");
	return false;
	//    return true;//(不启动)
}
//    当用户关闭虚拟键盘的时候回调函数
bool HouseScene::  onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
	CRoom room;
	std::string china;
	CCLOG("关闭输入");
	if (isLonging == false)
	{
		isLonging = true;
		
		if(m_pCmdStatus->createRoom(room,roomName) == RTN_SUCCESS)
		{
			china ="创建房间成功!";  

		}
		else
		{
			china="创建房间失败!";  
		}
	}  
	GBKToUTF8(china,"gbk","utf-8");    
	sender->setString(china.c_str());
	//pTitleLabel->setString("create room success");
	g_room = room;
	gotoRoom = true;
	return false;
	// return true;(不关闭)
}
//    当用户进行输入 虚拟键盘的时候回调函数
bool  HouseScene:: onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
	CCLOG("输入字符");
	if (*text  != 0x0d && *text != 0x0a)
	{
		roomName += text;
	}
	tf1->setColor(ccRED);
	tf1->setPlaceHolder(roomName.c_str());

	return false;
	//    return true;(不输入)
}
//    当用户进行删除文字 虚拟键盘的时候回调函数
bool HouseScene:: onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
	CCLOG("删除字符");
	return false;
	//    return true;(不删除)
}