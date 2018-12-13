#ifndef __LFTextWithBack_H__
#define __LFTextWithBack_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "textures/CCTextureCache.h"
#include "SimpleAudioEngine.h"

USING_NS_CC; 

class LFTextWithBack
{
public:
	LFTextWithBack(string content , CCLayer * layer);
	
	void showInPosition( float x, float y);
	void setString( string content);
	CCSize getContnetSize();

	CCLabelTTF * pLable;//ÎÄ×Ö 
	CCSprite* pSprite;//µ×É«

protected:
private:
};



#endif