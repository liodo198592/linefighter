#ifndef __IntroScene_SCENE_H__
#define __IntroScene_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "model_comm.h"

class CClientStatusLogIn;
USING_NS_CC; 


class IntroScene : public CCLayer ,public CCTextFieldDelegate
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    // implement the "static node()" method manually


    CREATE_FUNC(IntroScene);

	void  IntroScene::moveWithParabola(CCSprite* mSprite, 
								   CCPoint * startPoint,
                                   CCPoint * endPoint,
                                   float startAngle,
                                   float endAngle,
                                   float time);
	void IntroScene::myremove(CCNode *sender);
	void IntroScene::makeAtender(int sx,int sy, int dx, int dy , float t);
	virtual void myupdate(float tmd);
	virtual void realremove(CCNode *sender);

	CCTextFieldDelegate * pTextDelegate;
	CCTextFieldTTF* tf1;
	CClientStatusLogIn * m_pCmdStatus;
	string myName;
	bool isLonging;
	bool gotoNext;
};

#endif  // __IntroScene_SCENE_H__