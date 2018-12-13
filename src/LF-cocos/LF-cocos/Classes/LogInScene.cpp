#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "LogInScene.h"
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
#include "HouseScene.h"
#include "CharaSelectScene.h"
#include "IntroScene.h"

using namespace cocos2d;
char g_Control1Name[30];
int showKeyFlag = 0;

string g_userName;

#define MOVE_ACTION_TAG 123

CCScene* LogInScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        LogInScene *layer = LogInScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LogInScene::init()
{
	m_pCmdStatus = new CClientStatusLogIn();
    bool bRet = false;
	isLonging = false;
	gotoNext = false;
	forword = 0;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
		
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png","CloseSelected.png",this,menu_selector(LogInScene::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);
        
        pCloseItem->setPosition(size.width - size.width /32, size.width/32);
        
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);
        
        this->addChild(pMenu, 1);
        CCLabelTTF* pLabel = CCLabelTTF::create("LF V1.0", "Arial", 24);
        CC_BREAK_IF(! pLabel);
        
        pLabel->setPosition(ccp(size.width / 2, size.height - size.height/6));
        
        this->addChild(pLabel, 1);
        
        CCSprite* pSprite = CCSprite::create("map/Battlefield.png");
		float picwidth = pSprite->getContentSize().width;
		float picheight = pSprite->getContentSize().height;
		CCRect rectRoom(0,0,picwidth,picheight);
		pSprite->setTextureRect(rectRoom);//设置其为宽20，高20
		pSprite->setScaleX(size.width/picwidth);
		pSprite->setScaleY(size.height/picheight);
        CC_BREAK_IF(! pSprite);
        
        pSprite->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(pSprite, 0);
		//////////////////////////////////////////////////////////////////////////
		//菜单按钮
		CCMenuItemImage *pStoryItem = CCMenuItemImage::create("MenuButton/story.png","MenuButton/story-un.png",this,menu_selector(LogInScene::menuStoryCallback));
		CC_BREAK_IF(! pStoryItem);

		//菜单按钮
		CCMenuItemImage *pNetItem = CCMenuItemImage::create("MenuButton/net.png","MenuButton/net-un.png",this,menu_selector(LogInScene::menuNetCallback));
		CC_BREAK_IF(! pNetItem);

		//菜单按钮
		CCMenuItemImage *pintroItem = CCMenuItemImage::create("MenuButton/intro.png","MenuButton/intro-un.png",this,menu_selector(LogInScene::menuintroCallback));
		CC_BREAK_IF(! pintroItem);

		//菜单按钮
		CCMenuItemImage *paboutItem = CCMenuItemImage::create("MenuButton/about.png","MenuButton/about-un.png",this,menu_selector(LogInScene::menuaboutCallback));
		CC_BREAK_IF(! paboutItem);
		
		CCMenu *menu = CCMenu::create(pStoryItem,pNetItem,pintroItem,paboutItem,NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		CCPoint Storypoint(ccp(size.width/2, size.height/10));
		menu->setPosition(Storypoint);
		this->addChild(menu);


		//////////////////////////////////////////////////////////////////////////
		//加载人物
		CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("char/myrole.png"); 
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
		CCArray* animFrame = CCArray::createWithCapacity(13);
		cache->addSpriteFramesWithFile("char/myrole.plist");
		char str[100] = {0};  
		for(int i = 24; i < 36; i++)   
		{  
			sprintf(str, "00%02d.png", i);  
			CCSpriteFrame* frame = cache->spriteFrameByName( str );  
			animFrame->addObject(frame);  
		}  

		CCAnimation * animation = new CCAnimation();
		animation->initWithSpriteFrames(animFrame,0.1);

		shajiaSprite = CCSprite::createWithSpriteFrame( cache->spriteFrameByName("0024.png"));
		shajiaSprite->setScaleX(0.3);//宽缩小2倍 
		shajiaSprite->setScaleY(0.3);//高缩小2倍
		CCPoint point(size.width/32,size.height/3);
		shajiaSprite->setPosition(point);
		//shajiaSprite->setColor(ccRED);
		this->addChild(shajiaSprite,1);
		setTouchEnabled(true);
		CCAnimate * animate = CCAnimate::create(animation);
		shajiaSprite->runAction(CCRepeatForever::create(animate));
		CCPoint pointDest;
		pointDest.setPoint(size.width + shajiaSprite->getContentSize().width/2.0,130);
		CCFiniteTimeAction *actionMove = CCMoveTo::create(4,pointDest);
		shajiaSprite->runAction(actionMove);  
		schedule(schedule_selector(LogInScene::myupdate),2.0f);  
        bRet = true;
    } while (0);
    return bRet;
}

void LogInScene::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    //CCDirector::sharedDirector()->end();
}

void LogInScene::menuStoryCallback(CCObject* pSender)
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = CharaSelectScene::scene();
	CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
	pDirector->replaceScene(transitionScene);
}

void LogInScene::menuNetCallback(CCObject* pSender)
{
	std::string china="请输入名字，按回车结束";  
	GBKToUTF8(china,"gbk","utf-8");    
	tf1 = CCTextFieldTTF::textFieldWithPlaceHolder(china.c_str(),"Arial",30);
	tf1->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height/2));
	this->addChild(tf1);
	tf1->setDelegate(this);
	//開啟文字輸入框
	tf1->attachWithIME();
}
void LogInScene::menuintroCallback(CCObject* pSender)
{
	// "close" menu item clicked
	//CCDirector::sharedDirector()->end();
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = IntroScene::scene();
	CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
	pDirector->replaceScene(transitionScene);
}
void LogInScene::menuaboutCallback(CCObject* pSender)
{
	// "close" menu item clicked
	//CCDirector::sharedDirector()->end();
}

void LogInScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	//获取触点指针容器中第一个元素  
	CCTouch* ttouches = (CCTouch*)pTouches->anyObject();
	// 获得我们点击的坐标点
	CCPoint location = ttouches->getLocation();
	//设置精灵位置为触点位置。  
	//shajiaSprite->setPosition(location);  
	CCPoint pointDest(location);
	float x = 0, y = 0;
	shajiaSprite->getPosition(&x,&y);
	if (forword == 0)
	{
		if(x > pointDest.x)
		{
			shajiaSprite->setFlipX(true);
			forword = 1;
		}
	}
	else
	{
		if(x < pointDest.x)
		{
			shajiaSprite->setFlipX(false);
			forword = 0;
		}
	}

	CCAction* pAction = shajiaSprite->getActionByTag(MOVE_ACTION_TAG);
	if(pAction && !pAction->isDone())
	{
		shajiaSprite->stopAction(pAction);
	}
	
	CCFiniteTimeAction *actionMove2 = CCMoveTo::create(4,pointDest);
	actionMove2->setTag(MOVE_ACTION_TAG);
	shajiaSprite->runAction(actionMove2);   
}

bool LogInScene:: onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
	CCLOG("启动输入");
	return false;
	//    return true;//(不启动)
}
//    当用户关闭虚拟键盘的时候回调函数
bool LogInScene::  onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
	CCLOG("关闭输入");
	if (isLonging == false)
	{
		isLonging = true;
		if (m_pCmdStatus->login(myName) == RTN_SUCCESS)
		{
			std::string china="登陆成功!";   
			GBKToUTF8(china,"gbk","utf-8");    
				tf1->setString(china.c_str());
		}
		else
		{
			std::string china="登陆失败!";      
			GBKToUTF8(china,"gbk","utf-8");    
			tf1->setString(china.c_str());
		}
	}
	gotoNext = true;
	g_userName =  myName;
	return false;
	//    return true;(不关闭)
}

void LogInScene::myupdate(float tmd)
{
	if (gotoNext == true)
	{
		CCDirector *pDirector = CCDirector::sharedDirector();
		CCScene *pScene = HouseScene::scene();
		//CClientStatusHouse * pStatHourse = new CClientStatusHouse(g_userName);
		//((HouseScene *)pScene)->setCmdStatus(pStatHourse);
		CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
		pDirector->replaceScene(transitionScene);
	}
}
//    当用户进行输入 虚拟键盘的时候回调函数
bool  LogInScene:: onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
	CCLOG("输入字符");
	if (*text  != 0x0d && *text != 0x0a)
	{
		myName += text;
	}
	tf1->setColor(ccRED);
	tf1->setPlaceHolder(myName.c_str());
	
	return false;
	//    return true;(不输入)
}
//    当用户进行删除文字 虚拟键盘的时候回调函数
bool LogInScene:: onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
	CCLOG("删除字符");
	return false;
	//    return true;(不删除)
}

void LogInScene::setCmdStatus(CClientStatusLogIn * pCmdStatus)
{
	m_pCmdStatus = pCmdStatus;
}