#ifndef _CClientStatusLogIn_H_
#define _CClientStatusLogIn_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

class CCmdStatus;

class CClientStatusLogIn : public CCmdStatus
{
public:
	CClientStatusLogIn(){}
	virtual ~CClientStatusLogIn(){}
	virtual void runStatus(int sigal){return;}
	
	virtual void showInfo();
	virtual void showMenu();
	virtual int login(string strUsrName);
protected:
	//CClientStatusLogIn * m_pCmdStatus;
};


#endif