//#ifndef __TEXT_INPUT_TEST_H__
//#define __TEXT_INPUT_TEST_H__

////#include "../testBasic.h"
//#include "cocos2d.h"
//#include "../samples\TestCpp\Classes\VisibleRect.h"
//
//
//class KeyboardNotificationLayer;
//
//USING_NS_CC;
//
//class TextInputTest : public CCLayer
//{
//	KeyboardNotificationLayer * m_pNotificationLayer;
//public:
//	TextInputTest();
//
//	std::string title();
//	void addKeyboardNotificationLayer(KeyboardNotificationLayer * pLayer);
//
//	virtual void onEnter();
//
//	//获取KeyboardNotificationLayer
//	KeyboardNotificationLayer* GetKNL(){return m_pNotificationLayer;}
//};
//
////////////////////////////////////////////////////////////////////////////
//// KeyboardNotificationLayer for test IME keyboard notification.
////////////////////////////////////////////////////////////////////////////
//
//class KeyboardNotificationLayer : public CCLayer, public CCIMEDelegate
//{
//public:
//	KeyboardNotificationLayer();
//	static KeyboardNotificationLayer* createTextInputTest(int nIndex);
//
//	virtual std::string subtitle() = 0;
//	virtual void onClickTrackNode(bool bClicked) = 0;
//
//	virtual void registerWithTouchDispatcher();
//	virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
//
//	// CCLayer
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//
//	//设定位置
//	void SetPos(CCPoint _pos){pos=_pos; } 
//	//获取文本内容
//	char* GetContent();
//
//protected:
//	CCNode * m_pTrackNode;
//	CCPoint  m_beginPos;
//	CCPoint pos;
//
//};
//
////////////////////////////////////////////////////////////////////////////
//// TextFieldTTFDefaultTest for test TextFieldTTF default behavior.
////////////////////////////////////////////////////////////////////////////
//
//class TextFieldTTFDefaultTest : public KeyboardNotificationLayer
//{
//public:
//	// KeyboardNotificationLayer
//	virtual std::string subtitle();
//	virtual void onClickTrackNode(bool bClicked);
//
//	// CCLayer
//	virtual void onEnter();
//};
//
////////////////////////////////////////////////////////////////////////////
//// TextFieldTTFActionTest
////////////////////////////////////////////////////////////////////////////
//
//class TextFieldTTFActionTest : public KeyboardNotificationLayer, public CCTextFieldDelegate
//{
//	CCTextFieldTTF *    m_pTextField;
//	CCAction *          m_pTextFieldAction;
//	bool                m_bAction;
//	int                 m_nCharLimit;       // the textfield max char limit
//
//public:
//	void callbackRemoveNodeWhenDidAction(CCNode * pNode);
//
//	// KeyboardNotificationLayer
//	virtual std::string subtitle();
//	virtual void onClickTrackNode(bool bClicked);
//
//	// CCLayer
//	virtual void onEnter();
//	virtual void onExit();
//
//	// CCTextFieldDelegate
//	virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * pSender);
//	virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
//	virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
//	virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
//	virtual bool onDraw(CCTextFieldTTF * pSender);
//
//};
//
//
//
//#endif    // __TEXT_INPUT_TEST_H__
