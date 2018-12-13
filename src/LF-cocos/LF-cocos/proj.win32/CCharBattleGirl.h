#ifndef __CCharBattleGirl_H__
#define __CCharBattleGirl_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "textures/CCTextureCache.h"
#include "SimpleAudioEngine.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

USING_NS_CC; 

enum CCPDirection
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};


class CCharBattleGirl : public CCObject
{
public:
	CCharBattleGirl(CCLayer * layer,string name,CCPDirection direction = DIRECTION_RIGHT , int blood = MAX_HP);

	virtual void initFigure( CCLayer * layer);
	virtual void initMove();
	virtual void initFist();
	virtual void initAviod();
	virtual void initQigong();
	virtual void initQingzhu();
	virtual void initLife();
	virtual void initBisha();
	virtual void initDead();
	virtual void initBeattack();
	virtual void initBlood( CCLayer * layer);
	virtual CCAnimation * initanimation(char * pngpath,char * plistpath, int firstFrame , int lastFrame,float delay = 0.1);

	virtual void showmove();
	virtual void showfist();
	virtual void showaviod();
	virtual void showqigong(int iDirection);
	virtual void showqingzhu();
	virtual void showlife(int nowHp);
	virtual void showbisha();
	virtual void showdead();
	virtual void showbeattack();
	virtual void moveToPos(float x,float y);
	virtual void moveToPos(float dua, float x,float y);

	void stopMoveActionCallback();
	void setBodyColor(ccColor3B color);

	bool isShowingAction();

	void setPosition(float x, float y);
	void setFlipX(bool flag);
	void setBlood(float fBlood);
	void damageBlood(float fDamage);

	void setDirection(CCPDirection dir);
	float getPositionX();
	float getPositionY();
	float getContentSizeX();
	float getContentSizeY();
	
	float getCurrentHp();
	
private:
	void initLightBall(CCLayer * layer);

	//根据当前方向来判定是否需要翻转，bFlip为资源中人物的方向，若为朝左的人物则需传入true
	void handleSpritByDirection(CCPDirection sourceDir);

	void adjustShowBlood();
protected:
private:

	string m_strPlayerName;
	float m_fCurrentBlood;

	CCAnimation * animationMove;
	CCAnimation * animationFist;
	CCAnimation * animationAvoid;
	CCAnimation * animationQigong;
	CCAnimation * animationQingzhu;
	CCAnimation * animationbisha;
	CCAnimation * animationDead;
	CCAnimation * animationBeattack;
	CCAnimation * animationChangeBlood;

	CCNode* pNodePlayer;
	CCSprite * ccspritemove;
	CCSprite * ccspriteLife;
	CCSprite * ccspriteXue;

	CCAction * lastAction;

	CCParticleSystemQuad* m_pParticleLightBall1;//粒子光球1
	CCParticleSystemQuad* m_pParticleLightBall2;//粒子光球2

	float m_fXueCaoScale;//血槽的缩放比例

	CCPoint m_LightBall1Pos;
	CCPoint m_LightBall2Pos;

	CCPDirection m_Direction;
};



#endif