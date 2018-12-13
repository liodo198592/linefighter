#include "LFTextWithBack.h"
#include "cocoa/CCArray.h"
#include "textures/CCTextureCache.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "tool.h"


LFTextWithBack::LFTextWithBack(string content , CCLayer * layer)
{  
	GBKToUTF8(content,"gbk","utf-8");    
	pLable = CCLabelTTF::create(content.c_str(), "Arial", 14);
	CCSize textRoomsize = pLable->getContentSize();
	pSprite = CCSprite::create("MenuButton/textBackGroud.png");
	float picwidth = pSprite->getContentSize().width;
	float picheight = pSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	pSprite->setScaleX((textRoomsize.width+10)/picwidth);
	pSprite->setScaleY((textRoomsize.height+5)/picheight);
	pSprite->setAnchorPoint(ccp(0, 0)); 
	pLable->setAnchorPoint(ccp(0, 0)); 
	pSprite->setPosition(ccp(0,0));
	pLable->setPosition(ccp(0,0));
	pLable->setColor(ccRED);
	layer->addChild(pSprite);
	layer->addChild(pLable);
}


void LFTextWithBack::showInPosition( float x, float y)
{
	pSprite->setPosition(ccp(x,y));
	pLable->setPosition(ccp(x,y));
}

void LFTextWithBack::setString( string content)
{   
	GBKToUTF8(content,"gbk","utf-8");
	pLable->setString(content.c_str());
	CCSize textRoomsize = pLable->getContentSize();
	float picwidth = pSprite->getContentSize().width;
	float picheight = pSprite->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSprite->setTextureRect(rectRoom);//设置其为宽20，高20
	pSprite->setScaleX((textRoomsize.width+10)/picwidth);
	pSprite->setScaleY((textRoomsize.height+5)/picheight);
}

CCSize LFTextWithBack::getContnetSize()
{
	return pLable->getContentSize();
}