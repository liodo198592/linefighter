#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "IntroScene.h"
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
#include "ParabolaTo.h"


CCScene* IntroScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        IntroScene *layer = IntroScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool IntroScene::init()
{
	scheduleUpdate();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	do{
		CC_BREAK_IF(! CCLayer::init());
		//初始化背景
		CCSprite* pSprite = CCSprite::create("map/china.png");
		float picwidth = pSprite->getContentSize().width;
		float picheight = pSprite->getContentSize().height;
		pSprite->setScaleX(size.width/picwidth);
		pSprite->setScaleY(size.height/picheight);
		CC_BREAK_IF(! pSprite);

		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite, 0);
		int cnt = 0;
		//CCParticleMeteor特效

		schedule(schedule_selector(IntroScene::myupdate),0.05f);  
		//CCParticleExplosion特效

	}while(0);
    return true;
}

void IntroScene::myupdate(float tmd)
{
	int per = rand();
	if(per > 70)
	{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		int sx = rand()%((int)size.width-250 - 250 + 1) + 250;
		int sy = rand()%((int)size.height-250 - 250 + 1) + 250;
		int dx = rand()%((int)size.width-150 - 150 + 1) + 150;
		int dy = rand()%((int)size.height-150 - 150 + 1) + 150;
		float t = (rand()%((int)50 - 25 + 1) + 25)/(float)10;
		makeAtender(sx,sy,dx,dy ,t);
	}
}

void IntroScene::makeAtender(int sx,int sy, int dx, int dy , float t)
{
	int gh = rand()%(40 - 10 + 1) + 10;
	int ag = rand()%(100 - 60 + 1) + 60;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCParticleSystem * p6=CCParticleMeteor::create();
	p6->setTexture(CCTextureCache::sharedTextureCache()->addImage("t.jpg"));
	p6->setAutoRemoveOnFinish(true);
	p6->setScaleX(0.03);//宽缩小2倍 
	p6->setScaleY(0.03);//高缩小2倍
	p6->setRotation(260);
	CCPoint point(sx,sy);
	p6->setPosition(point);
	CCPoint pointDest;
	pointDest.setPoint(dx,dy);
	CCFiniteTimeAction *actionMove = CCMoveTo::create(4,pointDest);
	this->addChild(p6);        
	CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(IntroScene::myremove));
	CCFiniteTimeAction *actionOne = CCSequence::create(
    CCSpawn::create(CCRotateBy::create(t,90),ParabolaTo::create(t,point,pointDest,gh,ag),NULL),
	actionMoveDone,
	NULL);
	p6->runAction(actionOne);
}

void IntroScene::myremove(CCNode *sender)
{
	 CCParticleSystem *sprite = (CCParticleSystem *)sender;
	 
	 CCParticleSystem * p2=CCParticleFire::create();
	 p2->setTexture(CCTextureCache::sharedTextureCache()->addImage("t.jpg"));
	 p2->setAutoRemoveOnFinish(true);
	 p2->setScaleX(0.06);//宽缩小2倍 
	 p2->setScaleY(0.06);//高缩小2倍
	 p2->setPosition(sprite->getPosition());
	 this->addChild(p2);

	 this->removeChild(sprite, true);

	 CCFiniteTimeAction *actionMove = CCMoveTo::create(2,p2->getPosition());
	 CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(IntroScene::realremove));
	 CCFiniteTimeAction *actionOne = CCSequence::create(actionMove,actionMoveDone,NULL);
     p2->runAction(actionOne); 
	//this->removeChild();

	 
}

void IntroScene::realremove(CCNode *sender)
{
	 CCParticleSystem *sprite = (CCParticleSystem *)sender;
	 this->removeChild(sprite, true);
}
void IntroScene::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    //CCDirector::sharedDirector()->end();
}
