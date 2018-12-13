#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "model_comm.h"
#include "LFTextWithBack.h"
#include "CCharBattleGirl.h"

class CClientStatusGame;
class CCharBattleGirl;
USING_NS_CC; 
//USING_NS_CC_EXT;

class GameScene : public CCLayer 
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
	void menuStoryCallback(CCObject* pSender);
	void menuNetCallback(CCObject* pSender);
	void menuintroCallback(CCObject* pSender);
	void menuaboutCallback(CCObject* pSender);
    // implement the "static node()" method manually

    CREATE_FUNC(GameScene);

	void setCmdStatus(CClientStatusGame * pCmdStatus);
	void setUserName(string userName_x);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	CCTextFieldDelegate * pTextDelegate;
	CCTextFieldTTF* tf1;
	CClientStatusGame * m_pCmdStatus;
	string chatContent;
	bool isLonging;
	bool initfinished;

	//////////////////////////////////////////////////////////////////////////逻辑相关
	int m_selectedRoomID;
	//////////////////////////////////////////////////////////////////////////show相关
	LFTextWithBack* pTitleLabel;
	LFTextWithBack* pTitleChat;
	CCharBattleGirl* playerSprite[4];
	//////////////////////////////////////////////////////////////////////////menu
	virtual void menuChatCallback(CCObject* pSender);
	virtual void menuMoveCallback(CCObject* pSender);
	virtual void menuFistCallback(CCObject* pSender);
	virtual void menuQigongCallback(CCObject* pSender);
	virtual void menuBishaCallback(CCObject* pSender);
	virtual void menuHuanxianCallback(CCObject* pSender);
	virtual void menuShanbiCallback(CCObject* pSender);
	virtual void menuShuaiCallback(CCObject* pSender);
	//////////////////////////////////////////////////////////////////////////
	virtual void myupdate(float tmd);
};

#endif  // __GameScene_SCENE_H__