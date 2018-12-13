#include "MenuControler.h"

#define LEFT_BUTTON_QG	10
#define RIGHT_BUTTON_QG	11
#define LEFT_BUTTON_BS	12
#define RIGHT_BUTTON_BS	13
#define POS1_BUTTON	21
#define POS2_BUTTON	22
#define POS3_BUTTON	23

CMenuControler::CMenuControler(void)
{
}


CMenuControler::~CMenuControler(void)
{
}

bool CMenuControler::init()
{
	CCMenuItemImage *pChatItem = CCMenuItemImage::create("MenuButton/liaotian.png","MenuButton/liaotian2.png",this,menu_selector(CMenuControler::menuChatCallback));
	pChatItem->setScale(0.6);
	CCMenuItemImage *pMoveItem = CCMenuItemImage::create("MenuButton/move.png","MenuButton/move2.png",this,menu_selector(CMenuControler::menuMoveCallback));
	pMoveItem->setScale(0.6);
	CCMenuItemImage *pFistItem = CCMenuItemImage::create("MenuButton/quan.png","MenuButton/quan2.png",this,menu_selector(CMenuControler::menuFistCallback));
	pFistItem->setScale(0.6);
	CCMenuItemImage *pQigongItem = CCMenuItemImage::create("MenuButton/qigong.png","MenuButton/qigong2.png",this,menu_selector(CMenuControler::menuQigongCallback));
	pQigongItem->setScale(0.6);
	CCMenuItemImage *pBishaItem = CCMenuItemImage::create("MenuButton/bisha.png","MenuButton/bisha2.png",this,menu_selector(CMenuControler::menuBishaCallback));
	pBishaItem->setScale(0.6);
	CCMenuItemImage *pHuanxianItem = CCMenuItemImage::create("MenuButton/huanxian.png","MenuButton/huanxian2.png",this,menu_selector(CMenuControler::menuHuanxianCallback));
	pHuanxianItem->setScale(0.6);
	CCMenuItemImage *pShanbiItem = CCMenuItemImage::create("MenuButton/shan.png","MenuButton/shan2.png",this,menu_selector(CMenuControler::menuShanbiCallback));
	pShanbiItem->setScale(0.6);
	CCMenuItemImage *pShuaiItem = CCMenuItemImage::create("MenuButton/shuai.png","MenuButton/shuai2.png",this,menu_selector(CMenuControler::menuShuaiCallback));
	pShuaiItem->setScale(0.6);
	m_OpeMenuButton = CCMenu::create(pMoveItem,pFistItem,pQigongItem,pBishaItem,pHuanxianItem,pShanbiItem,pShuaiItem,NULL);
	m_OpeMenuButton->setAnchorPoint(ccp(0,0));
	m_OpeMenuButton->alignItemsHorizontallyWithPadding(20);
	m_OpeMenuButton->setPosition(ccp(250,50));
	this->addChild(m_OpeMenuButton,1);

	//创建气功向左、向右按钮
	CCMenuItemImage* iTemLeft = CCMenuItemImage::create("MenuButton/zuo.png","MenuButton/zuo2.png",this,menu_selector(CMenuControler::menuLeftRightCallBack));
	iTemLeft->setScale(0.6);
	iTemLeft->setTag(LEFT_BUTTON_QG);
	CCMenuItemImage* iTemRight = CCMenuItemImage::create("MenuButton/you.png","MenuButton/you2.png",this,menu_selector(CMenuControler::menuLeftRightCallBack));
	iTemRight->setScale(0.6);
	iTemRight->setTag(RIGHT_BUTTON_QG);
	m_pQiMenuLRButton = CCMenu::create(iTemLeft,iTemRight,NULL);
	m_pQiMenuLRButton->setAnchorPoint(ccp(0,0));
	m_pQiMenuLRButton->alignItemsHorizontallyWithPadding(20);
	m_pQiMenuLRButton->setPosition(ccp(190,80));
	this->addChild(m_pQiMenuLRButton,1);
	m_pQiMenuLRButton->setVisible(false);

	//创建必杀向左、向右按钮
	CCMenuItemImage* iTemLeftBS = CCMenuItemImage::create("MenuButton/zuo.png","MenuButton/zuo2.png",this,menu_selector(CMenuControler::menuLeftRightCallBack));
	iTemLeftBS->setScale(0.6);
	iTemLeftBS->setTag(LEFT_BUTTON_BS);
	CCMenuItemImage* iTemRightBS = CCMenuItemImage::create("MenuButton/you.png","MenuButton/you2.png",this,menu_selector(CMenuControler::menuLeftRightCallBack));
	iTemRightBS->setScale(0.6);
	iTemRightBS->setTag(RIGHT_BUTTON_BS);
	m_PBSMenuLRButton = CCMenu::create(iTemLeftBS,iTemRightBS,NULL);
	m_PBSMenuLRButton->setAnchorPoint(ccp(0,0));
	m_PBSMenuLRButton->alignItemsHorizontallyWithPadding(20);
	m_PBSMenuLRButton->setPosition(ccp(250,80));
	this->addChild(m_PBSMenuLRButton,1);
	m_PBSMenuLRButton->setVisible(false);

	//创建位置1、位置2、位置3按钮
	CCMenuItemImage* iTemPos1 = CCMenuItemImage::create("MenuButton/1-1.png","MenuButton/1-2.png",this,menu_selector(CMenuControler::menuPos123CallBack));
	iTemPos1->setScale(0.6);
	iTemPos1->setTag(POS1_BUTTON);
	CCMenuItemImage* iTemPos2 = CCMenuItemImage::create("MenuButton/2-1.png","MenuButton/2-2.png",this,menu_selector(CMenuControler::menuPos123CallBack));
	iTemPos2->setScale(0.6);
	iTemPos2->setTag(POS2_BUTTON);
	CCMenuItemImage* iTemPos3 = CCMenuItemImage::create("MenuButton/3-1.png","MenuButton/3-2.png",this,menu_selector(CMenuControler::menuPos123CallBack));
	iTemPos3->setScale(0.6);
	iTemPos3->setTag(POS3_BUTTON);
	m_PPos123MenuLRButton = CCMenu::create(iTemPos1,iTemPos2,iTemPos3,NULL);
	m_PPos123MenuLRButton->setAnchorPoint(ccp(0,0));
	m_PPos123MenuLRButton->alignItemsHorizontallyWithPadding(20);
	m_PPos123MenuLRButton->setPosition(ccp(100,80));
	this->addChild(m_PPos123MenuLRButton,1);
	m_PPos123MenuLRButton->setVisible(false);

	//创建执行、撤销按钮
	CCMenuItemImage* iExecItem = CCMenuItemImage::create("MenuButton/exec-1.png","MenuButton/exec-2.png",this,menu_selector(CMenuControler::execCallBack));
	iExecItem->setScale(1);
	CCMenuItemImage* iCancleItem = CCMenuItemImage::create("MenuButton/cancle-1.png","MenuButton/cancle-2.png",this,menu_selector(CMenuControler::cancleCallBack));
	iCancleItem->setScale(1);
	CCMenu* pEXCLButton = CCMenu::create(iExecItem,iCancleItem,NULL);
	pEXCLButton->setAnchorPoint(ccp(0,0));
	pEXCLButton->alignItemsHorizontallyWithPadding(20);
	pEXCLButton->setPosition(ccp(680,50));
	this->addChild(pEXCLButton);

	initStep();

	return true;
}

void CMenuControler::menuChatCallback(CCObject* pSender)
{

}
void CMenuControler::menuMoveCallback(CCObject* pSender)
{
	m_pQiMenuLRButton->setVisible(false);

	m_PBSMenuLRButton->setVisible(false);

	m_PPos123MenuLRButton->setVisible(!m_PPos123MenuLRButton->isVisible());
}
void CMenuControler::menuFistCallback(CCObject* pSender)
{
	if(m_vecAction.size() >= 3)
	{
		return;
	}
	CAction p((EACTION)EACTION_Hand);
	m_vecAction.push_back(p);
	updateStepShow();
}
void CMenuControler::menuQigongCallback(CCObject* pSender)
{
	m_PBSMenuLRButton->setVisible(false);
	m_PPos123MenuLRButton->setVisible(false);

	m_pQiMenuLRButton->setVisible(!m_pQiMenuLRButton->isVisible());
}
void CMenuControler::menuBishaCallback(CCObject* pSender)
{
	m_pQiMenuLRButton->setVisible(false);
	m_PPos123MenuLRButton->setVisible(false);

	m_PBSMenuLRButton->setVisible(!m_PBSMenuLRButton->isVisible());
}
void CMenuControler::menuHuanxianCallback(CCObject* pSender)
{
	if(m_vecAction.size() >= 3)
	{
		return;
	}

	CAction p((EACTION)EACTION_ChangeLine);
	m_vecAction.push_back(p);
	updateStepShow();
}
void CMenuControler::menuShanbiCallback(CCObject* pSender)
{
	if(m_vecAction.size() >= 3)
	{
		return;
	}
	CAction p((EACTION)EACTION_Avoid);
	m_vecAction.push_back(p);
	updateStepShow();
}
void CMenuControler::menuShuaiCallback(CCObject* pSender)
{
	if(m_vecAction.size() >= 3)
	{
		return;
	}
	CAction p((EACTION)EACTION_Fall);
	m_vecAction.push_back(p);
	updateStepShow();
}

void CMenuControler::menuLeftRightCallBack(CCObject* pSender)
{
	if(m_vecAction.size() >= 3)
	{
		return;
	}
	CCMenuItemImage* pImageSender = (CCMenuItemImage*)pSender;

	int tag = pImageSender->getTag();

	EACTION ectionId;
	int iDiraction;

	switch (tag)
	{
	case LEFT_BUTTON_QG:
		ectionId = EACTION_Air;
		iDiraction = 1;
		break;
	case RIGHT_BUTTON_QG:
		ectionId = EACTION_Air;
		iDiraction = 2;
		break;
	case LEFT_BUTTON_BS:
		ectionId = EACTION_Kill;
		iDiraction = 1;
		break;
	case RIGHT_BUTTON_BS:
		ectionId = EACTION_Kill;
		iDiraction = 2;
		break;
	default:
		return;
	}

	CAction p(ectionId);
	p.m_derection = iDiraction;
	m_vecAction.push_back(p);
	updateStepShow();

	m_pQiMenuLRButton->setVisible(false);
	m_PBSMenuLRButton->setVisible(false);

}
void CMenuControler::menuPos123CallBack(CCObject* pSender)
{
	if(m_vecAction.size() >= 3)
	{
		return;
	}
	CCMenuItemImage* pImageSender = (CCMenuItemImage*)pSender;

	int tag = pImageSender->getTag();

	//int iCurrentPosX = (m_pShowScreen->getMyPos() - 1)/3 + 1;
	int iCurrentPosX = 1;
	int iPosToY = 0;
	switch (tag)
	{
	case POS1_BUTTON:
		iPosToY = 1;
		break;
	case POS2_BUTTON:
		iPosToY = 2;
		break;
	case POS3_BUTTON:
		iPosToY = 3;
		break;
	default:
		return;
	}
	CAction p(EACTION_Move);
	p.m_destX = iCurrentPosX;
	p.m_destY = iPosToY;

	m_vecAction.push_back(p);
	updateStepShow();

	m_PPos123MenuLRButton->setVisible(false);
}

void CMenuControler::execCallBack(CCObject* pSender)
{
	if(m_vecAction.size() == 3)
	{
		m_inputExec = true;
	}
}
bool CMenuControler::getInputExecStat()
{
	return m_inputExec;
}
void CMenuControler::setInputExecFalse()
{
	m_inputExec = false;
}
void CMenuControler::cancleCallBack(CCObject* pSender)
{
	if(m_vecAction.size() <= 0)
	{
		return;
	}
	m_vecAction.pop_back();
	updateStepShow();
}

//隐藏操作按钮
void CMenuControler::hidMenuButton()
{
	m_OpeMenuButton->setVisible(false);
}
//显示操作按钮
void CMenuControler::showMenuButton()
{
	m_OpeMenuButton->setVisible(true);
}
void CMenuControler::initStep()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("MenuButton/menuButton.plist");

	m_stepSprites[0] = CCSprite::create();
	m_stepSprites[0]->setPosition(ccp(500,80));
	m_stepSprites[0]->setScale(0.3);
	m_stepSprites[1] = CCSprite::create();
	m_stepSprites[1]->setPosition(ccp(500,60));
	m_stepSprites[1]->setScale(0.3);
	m_stepSprites[2] = CCSprite::create();
	m_stepSprites[2]->setPosition(ccp(500,40));
	m_stepSprites[2]->setScale(0.3);
	for(int i=0;i<3;i++)
	{
		m_stepSprites[i]->setVisible(false);
		this->addChild(m_stepSprites[i]);
	}

}
void CMenuControler::updateStepShow()
{

	for(int i=0;i<3;i++)
	{
		m_stepSprites[i]->setVisible(false);
	}

	int i = 0;
	for(vector<CAction>::iterator it = m_vecAction.begin();it != m_vecAction.end();++it,++i)
	{
		string imgPath = getPisPathByAction(*it);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imgPath.c_str());
		m_stepSprites[i]->setDisplayFrame(frame);
		m_stepSprites[i]->setVisible(true);
	}
}
string CMenuControler::getPisPathByAction(const CAction& action)
{

	switch (action.m_eType)
	{
	case EACTION_Hand ://
		{
			return "quan.png";
			break;
		}
	case EACTION_Air ://气
		{
			return "qigong.png";
			break;
		}
	case EACTION_Fall://摔
		{
			return "shuai.png";
			break;
		}
	case EACTION_Move://移动
		{
			return "move.png";
			break;
		}
	case EACTION_ChangeLine://换线
		{
			return "huanxian.png";
			break;
		}
	case EACTION_Avoid://回避
		{
			return "shan.png";
			break;
		}
	case EACTION_Kill://必杀
		{
			return "bisha.png";
			break;
		}
	default:
		return "";
	}
}
vector<CAction> CMenuControler::getVecActions()
{
	return m_vecAction;
}
void CMenuControler::clearVecActions()
{
	m_vecAction.clear();
}