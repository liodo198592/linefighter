#ifndef _CEngineThread_H_
#define _CEngineThread_H_

#include "global.h"
#include "RingBuf.h"
#include "CProtocol.h"
#include "CUser.h"
#include "CEngine.h"

class CTCPServer;
class CPlayer;
class CAction;
class CUser;
class CEngine;

class CEngineThread : public ACE_Task<ACE_MT_SYNCH>
{
private:

public:
    CEngineThread(vector<CUser> vecUser_x , int roomID_x):m_engine(vecUser_x,roomID_x){}
    
	virtual ~CEngineThread() 	{}

	ACE_INT32 run();
	CEngine m_engine;
private:
	
	ACE_INT32 svc();
};






#endif