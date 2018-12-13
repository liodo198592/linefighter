#ifndef _CFLAI_H_
#define _CFLAI_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

//ÀÊª˙AI,≤‚ ‘engine π”√
class CPlayer;
class CAction;

class CFLAI
{
public:
	CFLAI(){}
	~CFLAI(){}
	void operator = (CFLAI room){}
	CFLAI(const CFLAI &room){}
public:
	virtual void getAction(vector<CAction>& subVec, CPlayer player,vector<CPlayer>& vplayers);
};

#endif