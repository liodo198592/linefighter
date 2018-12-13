#ifndef __LogInScene_SCENE_H__
#define __LogInScene_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "model_comm.h"

class CClientStatusLogIn;
USING_NS_CC; 


class LogInScene : public CCLayer ,public CCTextFieldDelegate
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

	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
	//    当用户关闭虚拟键盘的时候回调函数
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
	//    当用户进行输入 虚拟键盘的时候回调函数
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
	//    当用户进行删除文字 虚拟键盘的时候回调函数
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);

    CREATE_FUNC(LogInScene);

	void setCmdStatus(CClientStatusLogIn * pCmdStatus);
	virtual void myupdate(float tmd);

	int forword ; //0 向右，1 向左
	CCSprite * shajiaSprite;
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	CCTextFieldDelegate * pTextDelegate;
	CCTextFieldTTF* tf1;
	CClientStatusLogIn * m_pCmdStatus;
	string myName;
	bool isLonging;
	bool gotoNext;
};

#endif  // __LogInScene_SCENE_H__