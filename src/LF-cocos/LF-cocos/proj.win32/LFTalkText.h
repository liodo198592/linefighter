#ifndef __LFTalkText_H__
#define __LFTalkText_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "textures/CCTextureCache.h"
#include "SimpleAudioEngine.h"

USING_NS_CC; 

class LFTalkText
{
public:
	LFTalkText(string framePath, string photoPath, CCLayer * layer);
	
	void showInPosition( float x, float y);
	void setString( string content);
	CCSize getContnetSize();
	void open();
	void close();
	void setScale(float value);
	bool isopen();

	CCLabelTTF * pLable;//ÎÄ×Ö 
	CCSprite* pSpriteFrame;
	CCSprite* pSpritePhoto;
	CCLayer* showLayer;
	bool m_isopen;

protected:
private:
};



#endif