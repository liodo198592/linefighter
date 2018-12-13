#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "LogInScene.h"
#include "SimpleAudioEngine.h"
#include "TestGameScene.h"


using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(eglView);
	eglView->setDesignResolutionSize(800,450,kResolutionNoBorder);
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = LogInScene::scene();
	//CCScene* pScene = TestGameScene::scene();

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
