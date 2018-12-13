#include "CCharBattleGirl.h"
#include "cocoa/CCArray.h"
#include "textures/CCTextureCache.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "CCEGLView.h"
#include "AppDelegate.h"


#define PLAYER_WIDTH 
#define PLAYER_HIGHT 
#define BLOOD_OFFSET_X 0.56
#define BLOOD_OFFSET_Y 0.28

#define LIGHT_BALL_SIZE 32
#define LIGHTBALL1_POS_X 0	//粒子光球1的坐标在人物坐标中的比例X
#define LIGHTBALL1_POS_Y 8/9	//粒子光球1的坐标在人物坐标中的比例Y

#define LIGHTBALL2_POS_X 1	//粒子光球2的坐标在人物坐标中的比例X
#define LIGHTBALL2_POS_Y 8/9	//粒子光球2的坐标在人物坐标中的比例Y

#define MOVETOINTERVAL 0.5

#define MOVECACTIONTAG 20

CCharBattleGirl::CCharBattleGirl( CCLayer * layer,string name,CCPDirection direction , int blood):m_fCurrentBlood(blood),m_strPlayerName(name),m_Direction(direction)
{
	lastAction = NULL;
	initMove();
	initFist();
	initLife();
	initQigong();
	initQingzhu();
	initBisha();
	initAviod();
	initDead();
	initBeattack();

	
	initFigure(layer);
	

	initLightBall(layer);

	initBlood(layer);

	adjustShowBlood();
	
}

float CCharBattleGirl::getContentSizeX()
{
	return ccspritemove->boundingBox().size.width;
}

float CCharBattleGirl::getContentSizeY()
{
	return ccspritemove->boundingBox().size.height;
}
float CCharBattleGirl::getCurrentHp()
{
	return m_fCurrentBlood;
}
float CCharBattleGirl::getPositionX()
{
	return pNodePlayer->getPositionX();
}

float CCharBattleGirl::getPositionY()
{
	return pNodePlayer->getPositionY();
}


void CCharBattleGirl::setBodyColor(ccColor3B color)
{
	ccspritemove->setColor(color);
}
void CCharBattleGirl::initFigure( CCLayer * layer)
{
	CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("char/battalGirl.png"); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
	cache->addSpriteFramesWithFile("char/battalGirl.plist");
	ccspritemove = CCSprite::createWithSpriteFrame( cache->spriteFrameByName("0785.png"));
	//ccspritemove->setAnchorPoint(ccp(0,0));
	ccspritemove->setScale(0.4);//宽缩小2倍
	ccspritemove->setPosition(ccp(0,0));
	pNodePlayer = CCNode::create();
	pNodePlayer->addChild(ccspritemove);

	layer->addChild(pNodePlayer);
	handleSpritByDirection(DIRECTION_RIGHT);

}
void CCharBattleGirl::initLightBall(CCLayer * layer)
{
	CCRect playerSize = ccspritemove->boundingBox();

	m_pParticleLightBall1 = new CCParticleSystemQuad();
	m_pParticleLightBall1->initWithFile("5color.plist");

	m_pParticleLightBall1->setTextureWithRect(CCTextureCache::sharedTextureCache()->addImage("5color.plist.png"),CCRectMake(0,0,LIGHT_BALL_SIZE,LIGHT_BALL_SIZE));

	m_pParticleLightBall1->setBlendAdditive(true);

	m_LightBall1Pos.setPoint((0 - playerSize.size.width/2) + playerSize.size.width*LIGHTBALL1_POS_X,
		(0 - playerSize.size.height/2) + playerSize.size.height*LIGHTBALL1_POS_Y);
	m_pParticleLightBall1->setPosition(m_LightBall1Pos);

	pNodePlayer->addChild(m_pParticleLightBall1);

	m_pParticleLightBall2 = new CCParticleSystemQuad();
	m_pParticleLightBall2->initWithFile("5color.plist");

	m_pParticleLightBall2->setTextureWithRect(CCTextureCache::sharedTextureCache()->addImage("5color.plist.png"),CCRectMake(0,0,LIGHT_BALL_SIZE,LIGHT_BALL_SIZE));

	m_pParticleLightBall2->setBlendAdditive(true);

	m_LightBall2Pos.setPoint((0 - playerSize.size.width/2) + playerSize.size.width*LIGHTBALL2_POS_X,
		(0 - playerSize.size.height/2) + playerSize.size.height*LIGHTBALL2_POS_Y);
	m_pParticleLightBall2->setPosition(m_LightBall2Pos);

	pNodePlayer->addChild(m_pParticleLightBall2);

}
void CCharBattleGirl::initBlood(CCLayer * layer)
{
	CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("char/tool.png"); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
	cache->addSpriteFramesWithFile("char/tool.plist");
	ccspriteLife = CCSprite::createWithSpriteFrame( cache->spriteFrameByName("xuecao.png"));
	ccspriteXue = CCSprite::createWithSpriteFrame(cache->spriteFrameByName("xue.png"));
	CCRect xuecaoSize = ccspriteLife->boundingBox();
	CCRect xueSize = ccspriteXue->boundingBox();
	CCRect playerSize = ccspritemove->boundingBox();
	m_fXueCaoScale = playerSize.size.width/xuecaoSize.size.width;
	ccspriteLife->setScale(m_fXueCaoScale);
	ccspriteXue->setScale(m_fXueCaoScale);
	ccspriteXue->setAnchorPoint(ccp(0,0));
	xuecaoSize = ccspriteLife->boundingBox();
	xueSize = ccspriteXue->boundingBox();

	ccspriteLife->setPosition(ccp(0,playerSize.size.height/2 + xuecaoSize.size.height/2));
	ccspriteXue->setPosition(ccp(ccspriteLife->getPosition().x - xueSize.size.width/2 + (BLOOD_OFFSET_X - 0.5)*xuecaoSize.size.width,ccspriteLife->getPosition().y - xueSize.size.height/2+ (BLOOD_OFFSET_Y - 0.5)*xuecaoSize.size.height));

	pNodePlayer->addChild(ccspriteLife);
	pNodePlayer->addChild(ccspriteXue);


}
CCAnimation * CCharBattleGirl::initanimation(char * pngpath,char * plistpath, int firstFrame , int lastFrame,float delay)
{
	CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create(pngpath); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
	CCArray* animFrame = CCArray::createWithCapacity(lastFrame-firstFrame+1);
	cache->addSpriteFramesWithFile(plistpath);
	char str[100] = {0};  
	for(int i = firstFrame; i < lastFrame; i++)   
	{  
		sprintf(str, "%04d.png", i);  
		CCSpriteFrame* frame = cache->spriteFrameByName(str);  
		animFrame->addObject(frame);  
	}  
	CCAnimation * animation = new CCAnimation();
	animation->initWithSpriteFrames(animFrame,delay);
	return animation;
}

void CCharBattleGirl::initMove()
{
	animationMove = initanimation("char/battalGirl.png","char/battalGirl.plist",786,797);
}

void CCharBattleGirl::moveToPos(float x,float y)
{
	showmove();

	CCMoveTo* pMoveTo = CCMoveTo::create(MOVETOINTERVAL,ccp(x,y));

	CCCallFunc* call = CCCallFunc::create(this,callfunc_selector(CCharBattleGirl::stopMoveActionCallback));

	CCFiniteTimeAction* moveAction = CCSequence::create(pMoveTo,call,NULL);

	pNodePlayer->runAction(moveAction);
}

void CCharBattleGirl::moveToPos(float dua ,float x,float y)
{
	showmove();

	CCMoveTo* pMoveTo = CCMoveTo::create(dua,ccp(x,y));

	CCCallFunc* call = CCCallFunc::create(this,callfunc_selector(CCharBattleGirl::stopMoveActionCallback));

	CCFiniteTimeAction* moveAction = CCSequence::create(pMoveTo,call,NULL);

	pNodePlayer->runAction(moveAction);
}
void CCharBattleGirl::stopMoveActionCallback()
{
	ccspritemove->stopActionByTag(MOVECACTIONTAG);
}
bool CCharBattleGirl::isShowingAction()
{
	return (pNodePlayer->numberOfRunningActions() >0 
		|| ccspritemove->numberOfRunningActions() > 0);
}
void CCharBattleGirl::showmove()
{
	CCAnimate * animate = CCAnimate::create(animationMove);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	lastAction = CCRepeatForever::create(animate);
	lastAction->setTag(MOVECACTIONTAG);
	//ccspritemove->setFlipX(false);
	handleSpritByDirection(DIRECTION_RIGHT);
	ccspritemove->runAction(lastAction);


	return;
}

void CCharBattleGirl::initBeattack()
{
	animationBeattack = initanimation("char/battalGirl-beattack.png","char/battalGirl-beattack.plist",824,827);
}

void CCharBattleGirl::showbeattack()
{
	CCAnimate * animate = CCAnimate::create(animationBeattack);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	//lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(false);
	handleSpritByDirection(DIRECTION_RIGHT);
	ccspritemove->runAction(animate);

	CCScaleTo* pChangeAction = CCScaleTo::create(0.2,m_fXueCaoScale*(m_fCurrentBlood/MAX_HP),m_fXueCaoScale);
	ccspriteXue->runAction(pChangeAction);
	return;
}
void CCharBattleGirl::adjustShowBlood()
{
	ccspriteXue->setScale(m_fXueCaoScale*(m_fCurrentBlood/MAX_HP));
}

void CCharBattleGirl::setPosition(float x, float y)
{
	pNodePlayer->setPosition(ccp(x,y));
}
void CCharBattleGirl::setFlipX(bool flag)
{
	ccspritemove->setFlipX(flag);

}

void CCharBattleGirl::setDirection(CCPDirection dir)
{
	if(m_Direction == dir)
	{
		return;
	}
	m_Direction = (m_Direction == DIRECTION_LEFT)?DIRECTION_RIGHT:DIRECTION_LEFT;
}

//根据当前方向来判定是否需要翻转，bFlip为资源中人物的方向，若为朝左的人物则需传入true
void CCharBattleGirl::handleSpritByDirection(CCPDirection sourceDir)
{
	if(sourceDir == m_Direction)
	{
		ccspritemove->setFlipX(false);
	}
	else
	{
		ccspritemove->setFlipX(true);
	}
}
void CCharBattleGirl::setBlood(float fBlood)
{
	m_fCurrentBlood = fBlood;

	if(m_fCurrentBlood <=0)
	{
		m_fCurrentBlood = 0;
	}

	if(m_fCurrentBlood > MAX_HP)
	{
		m_fCurrentBlood = MAX_HP;
	}

}
void CCharBattleGirl::damageBlood(float fDamage)
{
	setBlood(m_fCurrentBlood - fDamage);
}
void CCharBattleGirl::initFist()
{
	animationFist = initanimation("char/battalGirl-kick.png","char/battalGirl-kick.plist",1301,1308);
}

void CCharBattleGirl::showfist()
{
	CCAnimate * animate = CCAnimate::create(animationFist);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(true);
	handleSpritByDirection(DIRECTION_LEFT);
	ccspritemove->runAction(animate);
	return;
}

void CCharBattleGirl::initAviod()
{
	animationAvoid = initanimation("char/battalGirl-avoid2.png","char/battalGirl-avoid2.plist",1296,1299);
}

void CCharBattleGirl::showaviod()
{
	CCAnimate * animate = CCAnimate::create(animationAvoid);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	//lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(true);
	handleSpritByDirection(DIRECTION_LEFT);
	ccspritemove->runAction(animate);
	return;
}


void CCharBattleGirl::initQigong()
{
	animationQigong = initanimation("char/battalGirl-qigong.png","char/battalGirl-qigong.plist",1349,1358,0.2);
}

void CCharBattleGirl::showqigong(int iDirection)
{
	CCAnimate * animate = CCAnimate::create(animationQigong);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(false);
	handleSpritByDirection(DIRECTION_RIGHT);
	ccspritemove->runAction(animate);


	CCPoint playerPos = ccspritemove->getPosition();
	CCRect playerSize = ccspritemove->boundingBox();
	CCSize totalSize = CCDirector::sharedDirector()->getWinSize();

	CCPoint aimPos;
	if(iDirection == 1)
	{
		aimPos = ccp(totalSize.width*1/10,pNodePlayer->convertToWorldSpace(ccspritemove->getPosition()).y);
	}
	else
	{
		aimPos = ccp(totalSize.width*9/10,pNodePlayer->convertToWorldSpace(ccspritemove->getPosition()).y);
	}
	aimPos = pNodePlayer->convertToNodeSpace(aimPos);
	//光球移动序列
	//CCPoint srcPos = m_pParticleLightBall1->getPosition();
	CCMoveTo* pLight1ToMiddle = CCMoveTo::create(0.2,playerPos);
	CCMoveTo* pLight1Launch = CCMoveTo::create(0.6,aimPos);
	CCMoveTo* pLight1LaunchReverse = CCMoveTo::create(0.3,playerPos);
	CCMoveTo* pLight1ToMiddleReverse = CCMoveTo::create(0.2,m_LightBall1Pos);
	CCArray actArry;
	actArry.addObject(pLight1ToMiddle);
	actArry.addObject(pLight1Launch);
	actArry.addObject(pLight1LaunchReverse);
	actArry.addObject(pLight1ToMiddleReverse);

	//CCPoint srcPos2 = m_pParticleLightBall2->getPosition();
	CCMoveTo* pLight2ToMiddle = CCMoveTo::create(0.3,playerPos);
	CCMoveTo* pLight2Launch = CCMoveTo::create(0.6,aimPos);
	CCMoveTo* pLight2LaunchReverse = CCMoveTo::create(0.3,playerPos);
	CCMoveTo* pLight2ToMiddleReverse = CCMoveTo::create(0.2,m_LightBall2Pos);
	CCArray actArry2;
	actArry2.addObject(pLight2ToMiddle);
	actArry2.addObject(pLight2Launch);
	actArry2.addObject(pLight2LaunchReverse);
	actArry2.addObject(pLight2ToMiddleReverse);


	CCFiniteTimeAction* pAct = CCSequence::create(&actArry);
	CCFiniteTimeAction* pAct2 = CCSequence::create(&actArry2);

	m_pParticleLightBall1->runAction(pAct);
	m_pParticleLightBall2->runAction(pAct2);



	return;
}

void CCharBattleGirl::initQingzhu()
{
	animationQingzhu = initanimation("char/battalGirl-qingzhu.png","char/battalGirl-qingzhu.plist",1375,1377);
}	

void CCharBattleGirl::showqingzhu()
{
	CCAnimate * animate = CCAnimate::create(animationQingzhu);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(false);
	handleSpritByDirection(DIRECTION_RIGHT);
	ccspritemove->runAction(animate);
	return;
}

void CCharBattleGirl::initLife()
{

}

void CCharBattleGirl::showlife(int nowHp)
{
	char str[100] = {0};
	int max = 997;
	int min = 948;
	int maxhp = 18;
	int rest = 948 + (997-948) * nowHp / 18;
	CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("char/LIFE.png"); 
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();  
	cache->addSpriteFramesWithFile("char/LIFE.plist");
	sprintf(str, "%04d.png", rest);  
	ccspriteLife->setDisplayFrame(cache->spriteFrameByName(str));
}

void CCharBattleGirl::initBisha()
{
	animationbisha = initanimation("char/battalGirl-bisha.png","char/battalGirl-bisha.plist",1309,1338);
}

void CCharBattleGirl::showbisha()
{
	CCAnimate * animate = CCAnimate::create(animationbisha);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(false);
	handleSpritByDirection(DIRECTION_RIGHT);
	ccspritemove->runAction(animate);
	return;
}

void CCharBattleGirl::initDead()
{
	animationDead = initanimation("char/battalGirl-dead.png","char/battalGirl-dead.plist",1312,1315);
}

void CCharBattleGirl::showdead()
{
	CCAnimate * animate = CCAnimate::create(animationDead);
	if (lastAction != NULL)
	{
		ccspritemove->stopAllActions();
	}
	lastAction = CCRepeatForever::create(animate);
	//ccspritemove->setFlipX(false);
	handleSpritByDirection(DIRECTION_RIGHT);
	ccspritemove->runAction(lastAction);
	return;
}