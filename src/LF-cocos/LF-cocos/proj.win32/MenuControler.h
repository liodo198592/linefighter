#ifndef _MENU_CONTROL_H_
#define _MENU_CONTROL_H_
#include "cocos2d.h"
USING_NS_CC;

#include "CAction.h"

class CMenuControler : public CCLayer
{
public:
	CMenuControler();
	~CMenuControler(void);

	virtual bool init();

	//���ز�����ť
	void hidMenuButton();
	//��ʾ������ť
	void showMenuButton();

	bool getInputExecStat();

	void setInputExecFalse();

	vector<CAction> getVecActions();

	void clearVecActions();

	void updateStepShow();

	CREATE_FUNC(CMenuControler);

private:
	virtual void menuChatCallback(CCObject* pSender);
	virtual void menuMoveCallback(CCObject* pSender);
	virtual void menuFistCallback(CCObject* pSender);
	virtual void menuQigongCallback(CCObject* pSender);
	virtual void menuBishaCallback(CCObject* pSender);
	virtual void menuHuanxianCallback(CCObject* pSender);
	virtual void menuShanbiCallback(CCObject* pSender);
	virtual void menuShuaiCallback(CCObject* pSender);

	virtual void menuLeftRightCallBack(CCObject* pSender);
	virtual void menuPos123CallBack(CCObject* pSender);

	virtual void execCallBack(CCObject* pSender);

	virtual void cancleCallBack(CCObject* pSender);

	string getPisPathByAction(const CAction& action);

	void initStep();





private:
	//������ť
	CCMenu* m_OpeMenuButton;

	//���������Ұ�ť
	CCMenu* m_pQiMenuLRButton;

	//��ɱ���Ұ�ť
	CCMenu* m_PBSMenuLRButton;

	//λ��123��ť
	CCMenu* m_PPos123MenuLRButton;

	vector<CAction> m_vecAction;

	CCSprite* m_stepSprites[3];

	bool m_inputExec;
};


#endif