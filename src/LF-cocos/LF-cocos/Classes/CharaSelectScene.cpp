#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CharaSelectScene.h"
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
#include "ChrisScene1.h"
#include "RoomScene.h"
#include "LFTextWithBack.h"

using namespace cocos2d;

CCScene* CharaSelectScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CharaSelectScene *layer = CharaSelectScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CharaSelectScene::init()
{
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
			CCSprite* pSprite = CCSprite::create("map/CharaSelect.png");
			float picwidth = pSprite->getContentSize().width;
			float picheight = pSprite->getContentSize().height;
			pSprite->setScaleX(size.width/picwidth);
			pSprite->setScaleY(size.height/picheight);
			CC_BREAK_IF(! pSprite);

			pSprite->setPosition(ccp(size.width/2, size.height/2));
			this->addChild(pSprite, 0);
		}

		CCSize visibSize=CCDirector::sharedDirector()->getVisibleSize();  
		UserTableView=cocos2d::extension::CCTableView::create(this, CCSizeMake(size.width/1.6, size.height/8));  
		UserTableView->setDirection(cocos2d::extension::kCCScrollViewDirectionHorizontal);  
		UserTableView->setAnchorPoint(ccp(0, 0)); 
		UserTableView->setPosition(size.width/3.2,size.height/1.4);
		UserTableView->setDelegate(this);
		UserTableView->setVerticalFillOrder(cocos2d::extension::kCCTableViewFillTopDown);  
		this->addChild(UserTableView,2);  
		UserTableView->reloadData();//读取数据
		UserTableView->setTag(1);

		{
			pTitleLabel = new LFTextWithBack("请选择使用的角色",this);
			CCSize textsize = pTitleLabel->getContnetSize();
			pTitleLabel->showInPosition(size.width/128,size.height-textsize.height);
		}
		
		{
			pCharInfo = new LFTextWithBack("角色介绍",this);
			CCSize textsize = pCharInfo->getContnetSize();
			pCharInfo->showInPosition(size.width/128,size.height-size.height/4-textsize.height);
		}
		
		//任务介绍

		{
			//加载人物
			CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("char/CharaSelect-BattleGirl.png"); 
			CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
			CCArray* animFrame = CCArray::createWithCapacity(14);
			cache->addSpriteFramesWithFile("char/CharaSelect-BattleGirl.plist");
			char str[100] = {0};  
			for(int i = 2028; i < 2042; i++)   
			{  
				sprintf(str, "%04d.png", i);  
				CCSpriteFrame* frame = cache->spriteFrameByName( str );  
				animFrame->addObject(frame);  
			}  

			animation = new CCAnimation();
			animation->initWithSpriteFrames(animFrame,0.1);

			rolePhotoSprite = CCSprite::createWithSpriteFrame( cache->spriteFrameByName("2028.png"));
			rolePhotoSprite->setScale(0.3);//宽缩小2倍 
			rolePhotoSprite->setPosition(ccp(size.width/2-size.width/6.4, size.height*1/2));
			this->addChild(rolePhotoSprite,1);
			setTouchEnabled(true);
			
		}
		{
			//加载人物
			CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("char/CharaSelect-BattleGirl2.png"); 
			CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
			CCArray* animFrame = CCArray::createWithCapacity(13);
			cache->addSpriteFramesWithFile("char/CharaSelect-BattleGirl2.plist");
			char str[100] = {0};  
			for(int i = 2015; i < 2027; i++)   
			{  
				sprintf(str, "%04d.png", i);  
				CCSpriteFrame* frame = cache->spriteFrameByName( str );  
				animFrame->addObject(frame);  
			}  

			animation2 = new CCAnimation();
			animation2->initWithSpriteFrames(animFrame,0.1);

			rolePhotoSprite2 = CCSprite::createWithSpriteFrame( cache->spriteFrameByName("2015.png"));
			rolePhotoSprite2->setScale(0.3);//宽缩小2倍 
			rolePhotoSprite2->setPosition(ccp(size.width/2+size.width/3.5, size.height*1/2));
			this->addChild(rolePhotoSprite2,1);
			setTouchEnabled(true);
		}
		//////////////////////////////////////////////////////////////////////////开始按钮
		pStartItem = CCMenuItemImage::create("MenuButton/start.png","MenuButton/start-un.png",this,menu_selector(CharaSelectScene::menuStartCallback));
		pStartItem ->setScale(0.6);
		pStartItem->setAnchorPoint(ccp(0,0));
		menu = CCMenu::create(pStartItem,NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(-size.width/12.8,-size.height/6.4));
		this->addChild(menu,1);
		//////////////////////////////////////////////////////////////////////////
		schedule(schedule_selector(CharaSelectScene::myupdate),1.0f);  
		initfinished = true;
        bRet = true;
    } while (0);
    return bRet;
}

void CharaSelectScene::menuStartCallback(CCObject* pSender)
{
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCScene *pScene = ChrisScene1::scene();
	CCTransitionCrossFade  *transitionScene = CCTransitionCrossFade ::create(1.5,pScene);
	pDirector->replaceScene(transitionScene);
}

void CharaSelectScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{   
}


void CharaSelectScene::myupdate(float tmd)
{
	UserTableView->reloadData();
}

unsigned int CharaSelectScene::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)  
{  
	//获取数据多少
	int num = 0;
	int tag = table->getTag();
	num = 2;
	return num;  
}  

cocos2d::extension::CCTableViewCell* CharaSelectScene::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)  
{  
	float with = 120;
	CCString *nameString;
	int tagindex = 0;
	int roomid = 0;
	int tag = table->getTag();
	string showStr = "";
	if (idx == 0)
	{
		showStr = "      克里斯";
	}
	else if (idx == 1)
	{
		showStr = "      安吉丽娜";
	}
	tagindex = idx;   
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


CCSize CharaSelectScene::cellSizeForTable(cocos2d::extension::CCTableView *table)  
{  
	return CCSizeMake(250, 24);  
}  

void CharaSelectScene::tableCellHighlight(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLOG("wwww");  
	m_selectedRoomID = cell->getIdx();
	CCTexture2D *aTexturex=CCTextureCache::sharedTextureCache()->addImage("MenuButton/textBackGroud-selected.png");  
	int tag = table->getTag();
	float with = 120;
	CCSprite *pSpritex=(CCSprite *)cell->getChildByTag(cell->getTag());  
	float picwidth = pSpritex->getContentSize().width;
	float picheight = pSpritex->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSpritex->setTexture(aTexturex);  
	pSpritex->setTextureRect(rectRoom);//设置其为宽20，高20
	pSpritex->setScaleX((with)/picwidth);
	pSpritex->setScaleY((20)/picheight);
}  

void CharaSelectScene::tableCellUnhighlight(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLOG("dddd");  
	CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("MenuButton/textBackGroud.png");  
	int tag = table->getTag();
	float with = 120;
	CCSprite *pSprite=(CCSprite *)cell->getChildByTag(cell->getTag());  
	float picwidth = pSprite->getContentSize().width;
	float picheight = pSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSprite->setTexture(aTexture);  
	pSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	pSprite->setScaleX((with)/picwidth);
	pSprite->setScaleY((20)/picheight);
}  


void CharaSelectScene::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell)  
{  
	CCLog("cell touched at index: %i", cell->getIdx());
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (cell->getIdx() == 0)
	{
		rolePhotoSprite->stopAllActions();
		rolePhotoSprite2->stopAllActions();
		animate = CCAnimate::create(animation);
		rolePhotoSprite->runAction(CCRepeatForever::create(animate));
		menu->setPosition(ccp(size.width/2.78,size.height/10));
		string info = "人物介绍：\n克里斯,无忧无虑\n的小镇少女,天性乐\n观有一个爱护自\n己的姐姐，无意\n中被卷入了一个\n巨大的阴谋。。。";
		pCharInfo->setString(info);
		CCSize textsize = pCharInfo->getContnetSize();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pCharInfo->showInPosition(5,size.height-70-textsize.height);
	}
	else
	{
		rolePhotoSprite->stopAllActions();
		rolePhotoSprite2->stopAllActions();
		animate2 = CCAnimate::create(animation2);
		rolePhotoSprite2->runAction(CCRepeatForever::create(animate2));
		menu->setPosition(ccp(size.width/1.42,size.height/10));
		string info = "人物介绍：\n安吉丽娜 ,拥有\n杰出的战斗能力\n，王国禁卫军的\n侦察部队队长\n，在一次执行任\n务过程中发现\n了自己具有隐\n藏的能力。。。";
		pCharInfo->setString(info);
		CCSize textsize = pCharInfo->getContnetSize();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pCharInfo->showInPosition(5,size.height-70-textsize.height);
	}
	//m_selectedRoomID = cell->getTag();
	//tableCellHighlight(table,cell);
}  

void CharaSelectScene::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
	CCLOG("scroll");
}

void CharaSelectScene::scrollViewDidZoom(cocos2d::extension::CCScrollView* view)
{
	CCLOG("zoom");
}

void CharaSelectScene::scrollBar(cocos2d::extension::CCTableView* table)
{
	return;
}