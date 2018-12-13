#ifndef __HouseScene_SCENE_H__
#define __HouseScene_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "model_comm.h"
#include "LFTextWithBack.h"

class CClientStatusHouse;
USING_NS_CC; 
//USING_NS_CC_EXT;

class HouseScene : public CCLayer , public cocos2d::extension::CCTableViewDelegate,public cocos2d::extension::CCTableViewDataSource ,public CCTextFieldDelegate
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

    CREATE_FUNC(HouseScene);

	void setCmdStatus(CClientStatusHouse * pCmdStatus);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	CCTextFieldDelegate * pTextDelegate;
	CCTextFieldTTF* tf1;
	CClientStatusHouse * m_pCmdStatus;
	string roomName;
	bool isLonging;
	bool initfinished;
	bool gotoRoom;

	//////////////////////////////////////////////////////////////////////////逻辑相关
	int m_selectedRoomID;
	//////////////////////////////////////////////////////////////////////////show相关
	cocos2d::extension::CCTableView *UserTableView;
	cocos2d::extension::CCTableView *UserRoomView;
	LFTextWithBack* pTitleUserLabel;
	LFTextWithBack* pTitleLabel;
	LFTextWithBack* pTitleRoomLabel;
	//////////////////////////////////////////////////////////////////////////
	//CCTableViewDelegate继承自CCScrollViewDelegate  
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);  
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);  
	//点击哪个cell 
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);  
	//每个cell的size 
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);  
	//生成cell 
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);  
	//cell的数量 
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);  
	//按下去的时候，就是高亮显示，这里可以设置高亮状态 
	virtual void tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);  
	//松开的时候，取消高亮状态 
	virtual void tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);  
	void scrollBar(cocos2d::extension::CCTableView* table);  
	//////////////////////////////////////////////////////////////////////////Timer


	//////////////////////////////////////////////////////////////////////////menu
	virtual void menuExitCallback(CCObject* pSender);
	virtual void menuJionCallback(CCObject* pSender);
	virtual void menuCreateCallback(CCObject* pSender);
	//////////////////////////////////////////////////////////////////////////input
	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender);
	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender);
	virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
	//////////////////////////////////////////////////////////////////////////
	virtual void myupdate(float tmd);
};

#endif  // __HouseScene_SCENE_H__