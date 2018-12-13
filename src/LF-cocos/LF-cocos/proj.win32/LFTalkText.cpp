#include "LFTalkText.h"
#include "cocoa/CCArray.h"
#include "textures/CCTextureCache.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "tool.h"


LFTalkText::LFTalkText(string framePath, string photoPath, CCLayer * layer)
{
	showLayer = layer;
	pLable = CCLabelTTF::create(" ", "Arial", 14);
	CCSize textRoomsize = pLable->getContentSize();
	pSpriteFrame = CCSprite::create(framePath.c_str());
	pSpritePhoto = CCSprite::create(photoPath.c_str());
	float picwidth = pSpriteFrame->getContentSize().width;
	float picheight = pSpriteFrame->getContentSize().height;
	CCRect rectRoom(0,0,picwidth,picheight);
	pSpriteFrame->setTextureRect(rectRoom);
	pSpriteFrame->setScale(0.5);
	float picwidth2 = pSpritePhoto->getContentSize().width;
	float picheight2 = pSpritePhoto->getContentSize().height;
	CCRect rectRoom2(0,0,picwidth2,picheight2);
	pSpritePhoto->setTextureRect(rectRoom2);

	pSpriteFrame->setAnchorPoint(ccp(0, 0)); 
	pSpritePhoto->setAnchorPoint(ccp(0, 0)); 
	pLable->setAnchorPoint(ccp(0, 0)); 
	showLayer->addChild(pSpriteFrame);
	showLayer->addChild(pSpritePhoto);
	showLayer->addChild(pLable);
	m_isopen = false;
}

void LFTalkText::setScale(float value)
{
	pSpriteFrame->setScale(value);
	pSpritePhoto->setScale(value);
	pLable->setScale(value);
}

void LFTalkText::open()
{
	CCSize textRoomsize = pLable->getContentSize();
	float picwidth = pSpriteFrame->getContentSize().width;
	float picheight = pSpriteFrame->getContentSize().height;
	pSpriteFrame->setScaleX((textRoomsize.width+80)/picwidth);
	CCScaleTo* scaleTo = CCScaleTo::create(0.15, (textRoomsize.width+80)/picwidth*1.2,0.6);
	CCScaleTo* scaleTo2 = CCScaleTo::create(0.15, (textRoomsize.width+80)/picwidth,0.5);
	CCScaleTo* scaleTo3 = CCScaleTo::create(0.15, 1.2);
	CCScaleTo* scaleTo4 = CCScaleTo::create(0.15, 1);
	CCScaleTo* scaleTo5 = CCScaleTo::create(0.15, 1.2);
	CCScaleTo* scaleTo6 = CCScaleTo::create(0.15, 1);
	pSpriteFrame->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo2));
	pLable->runAction(CCSequence::createWithTwoActions(scaleTo3,scaleTo4));
	pSpritePhoto->runAction(CCSequence::createWithTwoActions(scaleTo5,scaleTo6));
	m_isopen = true;
}

bool LFTalkText::isopen()
{
	return m_isopen;
}

void LFTalkText::close()
{
	CCSize textRoomsize = pLable->getContentSize();
	float picwidth = pSpriteFrame->getContentSize().width;
	float picheight = pSpriteFrame->getContentSize().height;
	pSpriteFrame->setScaleX((textRoomsize.width+80)/picwidth);
	CCScaleTo* scaleTo = CCScaleTo::create(0.15, 0);
	CCScaleTo* scaleTo2 = CCScaleTo::create(0.15, 0);
	CCScaleTo* scaleTo3 = CCScaleTo::create(0.15, 0);
	CCScaleTo* scaleTo4 = CCScaleTo::create(0.15, 0);
	CCScaleTo* scaleTo5 = CCScaleTo::create(0.15, 0);
	CCScaleTo* scaleTo6 = CCScaleTo::create(0.15, 0);
	pSpriteFrame->runAction(CCSequence::createWithTwoActions(scaleTo,scaleTo2));
	pLable->runAction(CCSequence::createWithTwoActions(scaleTo3,scaleTo4));
	pSpritePhoto->runAction(CCSequence::createWithTwoActions(scaleTo5,scaleTo6));
	m_isopen = false;
}

void LFTalkText::showInPosition( float x, float y)
{
	float picwidth = pSpriteFrame->getContentSize().width*0.5;
	float picheight = pSpriteFrame->getContentSize().height*0.5;
	pSpriteFrame->setPosition(ccp(x,y));
	pLable->setPosition(ccp(x+40,y+picheight/2));
	pSpritePhoto->setPosition(ccp(x,y+picheight-30));
}

void LFTalkText::setString( string content)
{  
	GBKToUTF8(content,"gbk","utf-8");    
	pLable->setString(content.c_str());
	CCSize textRoomsize = pLable->getContentSize();
	float picwidth = pSpriteFrame->getContentSize().width;
	float picheight = pSpriteFrame->getContentSize().height;
	pSpriteFrame->setScaleX((textRoomsize.width+80)/picwidth);
}

CCSize LFTalkText::getContnetSize()
{
	return pLable->getContentSize();
}