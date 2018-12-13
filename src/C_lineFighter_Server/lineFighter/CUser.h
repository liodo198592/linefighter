#ifndef _CUser_H_
#define _CUser_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

#define CUSER_READY 1
#define CUSER_UNREADY 0
#define CUSER_GAMMING 2
//单例house
class CUser
{
protected:
	string userName;
	int isReady; //0表示未准备好  1 表示已准备好
public:
	CUser()
	{
		this->isReady = CUSER_UNREADY;
	}
	CUser(string userName_x):userName(userName_x){}
	CUser(const CUser &user)
	{
		this->userName = user.userName;
		this->isReady = user.isReady;
	}
	void operator = (CUser user)
	{
		this->userName = user.userName;
		this->isReady = user.isReady;
	}
	virtual ~CUser(){}
	
	virtual string getUserName()
	{
		return userName;
	}
	virtual void setUserName(string userName_x)
	{
		userName = userName_x;
	}

	virtual int getReady()
	{
		return isReady;
	}
	virtual void setReady(int isReady_x)
	{
		isReady = isReady_x;
	}

};








#endif