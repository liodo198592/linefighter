#ifndef _CClientStatus_H_
#define _CClientStatus_H_

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "CChatInfo.h"

enum ESignal
{
	E_false = 0,
	E_true ,
	E_TIMEOUT ,
};


class CCmdStatus
{
public:
	CCmdStatus(){}
	virtual ~CCmdStatus(){}
	virtual void runStatus(int sigal){return;}

	virtual void showInfo(){return;}
	virtual void showMenu(){return;}

	virtual int recvAReportFormServer(int needCode ,unsigned char * pbuf , int& len);

	int key_getMenuButten(int& MenuID , int timeout);
	int key_getString(string& str , int timeout);
	int key_getInt(int& intval , int timeout);

protected:
	queue<CChatInfo> qChatqueue;
	//CCmdStatus * m_pCmdStatus;
};


#endif