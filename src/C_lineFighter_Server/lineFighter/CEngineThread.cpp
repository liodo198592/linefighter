#include "CEngineThread.h"
#include "global.h"
 
ACE_INT32 CEngineThread::run()
{
	activate(THR_NEW_LWP,1);
	return 0;
}

ACE_INT32 CEngineThread::svc()
{
	m_engine.svc();
	return 0;
}