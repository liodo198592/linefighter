#ifndef __ChrisScene1_SCENE_H__
#define __ChrisScene1_SCENE_H__

//选择角色

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "model_comm.h"
#include "LFTextWithBack.h"
#include "LFTalkText.h"
#include "CCharBattleGirl.h"
#include "pthread.h"
#ifdef MAKE_ANDRIOD
#include "CEngine.h"
#else
#include "../../../C_lineFighter_Server/lineFighter/CEngine.h"
#endif

#ifdef MAKE_ANDRIOD
#include "ClientStatusGameBase.h"
#else
#include "LFClient\ClientStatusGameBase.h"
#endif

#include "MenuControler.h"
#include "ShowScreen.h"

#include "StateHandler.h"

class CClientStatusSingle;
USING_NS_CC; 
//USING_NS_CC_EXT;

enum EShowType
{
	EShowType_Move = 1,
	EShowType_Talk,
	EShowType_TitleText,
};

struct SStoryTalkInfo
{
	EShowType eType;
	string name;
	string info;
	int destX;
	int destY;
	//EShowType_TitleText
	SStoryTalkInfo(EShowType eType_x,string info_p){
		eType = eType_x;
		info = info_p;
	}
	//EShowType_Talk
	SStoryTalkInfo(EShowType eType_x,string name_p,string info_p){
		eType = eType_x;
		name = name_p;
		info = info_p;
	}
	//EShowType_TitleText
	SStoryTalkInfo(EShowType eType_x,string name_p,int x, int y){
		eType = eType_x;
		name = name_p;
		destX = x;
		destY = y;
	}
};

class ChrisScene1 : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuStartCallback(CCObject* pSender);
    // implement the "static node()" method manually

    CREATE_FUNC(ChrisScene1);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	CCTextFieldDelegate * pTextDelegate;
	CCTextFieldTTF* tf1;
	CCLabelTTF * titleLable; 
	string roomName;
	bool isLonging;
	bool initfinished;
	bool gotoRoom;
	//////////////////////////////////////////////////////////////////////////对话
	map<int,vector<SStoryTalkInfo> > vTalkString;//key是回合号，0表示在第一回合开始前就播放，其余表示对应回合号到了开始播放，name对应的是
	vector<string> vTileString;
	map<string,CCharBattleGirl *> vChars;
	map<string,LFTalkText *> pTalkTextChris;
	int talkindex;
	//////////////////////////////////////////////////////////////////////////逻辑相关
	int m_selectedRoomID;
	//////////////////////////////////////////////////////////////////////////show相关
	cocos2d::extension::CCTableView *UserTableView;
	cocos2d::extension::CCTableView *UserRoomView;
	LFTextWithBack* pTitleUserLabel;
	LFTextWithBack* pTitleLabel;
	LFTextWithBack* pCharInfo;
	CEngine * engin;
	CClientStatusGameBase * m_pCmdStatus;
	//////////////////////////////////////////////////////////////////////////Timer
	CCSprite * rolePhotoSprite;
	CCAnimate * animate;
	CCAnimation * animation;
	CCSprite * rolePhotoSprite2;
	CCAnimate * animate2;
	CCAnimation * animation2;
	CCMenuItemImage *pStartItem;
	CCMenu *menu;

	int gamestart;
	pthread_t pidrun;
	
	//////////////////////////////////////////////////////////////////////////menu
	
	virtual void myupdate(float tmd);
	virtual bool storyinit();
	static void * runEngineSvc(void * p);

	void handleStatUpdate(float tmd);
	void initBattleScene();

private:

	CMenuControler* m_pMenuControl;

	CShowScreen* m_pShowScreen;

	CStateHandler* m_PStateHandler;

	bool m_finishInitBattle;
};

#endif  // __ChrisScene1_SCENE_H__