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
	//    ���û��ر�������̵�ʱ��ص�����
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
	//    ���û��������� ������̵�ʱ��ص�����
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
	//    ���û�����ɾ������ ������̵�ʱ��ص�����
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);

    CREATE_FUNC(LogInScene);

	void setCmdStatus(CClientStatusLogIn * pCmdStatus);
	virtual void myupdate(float tmd);

	int forword ; //0 ���ң�1 ����
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