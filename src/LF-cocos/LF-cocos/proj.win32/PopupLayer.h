#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"

//class PopupLayer: public CCLayer{
//public:
//	PopupLayer();
//	~PopupLayer();
//
//	virtual bool init();
//	CREATE_FUNC(PopupLayer);
//
//	// ��Ҫ��д����ע�ắ�������¸�����������
//	virtual void registerWithTouchDispatcher(void);
//	// ��д������������Զ���� true �����������㣬�ﵽ ��ģ̬�� Ч��
//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//
//	// ���ܣ������öԻ��򱳾�ͼƬ
//	static PopupLayer* create(const char* backgroundImage);
//
//	// ��������ʾ���⣬�����趨��ʾ���ִ�С
//	void setTitle(const char* title, int fontsize = 20);
//	// �ı����ݣ�padding Ϊ���ֵ��Ի�������Ԥ���ľ��룬���ǿɿصģ����Ϸ��ľ����������
//	void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);
//
//	// �ص��������������ť�����ǹرյ������ͬ�£���Ҫһ���ص���������֪ͨ���ǵ�����ĸ���ť������ж����
//	void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);
//
//	// Ϊ����Ӱ�ť���棬��װ��һ������������Щ��Ҫ�Ĳ���
//	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
//
//	// Ϊ������ʾ��ʱ֮ǰ��������Ч��ѡ���� onEnter �ﶯ̬չʾ
//	virtual void onEnter();
//	virtual void onExit();
//
//};