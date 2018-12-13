//////////////////////////////////////////////////////////////////////////
//abstract     : 日志截断线程
//author:      : 周宇
//create date  : 2010-03-06
//modify date  : 2010-03-06
//////////////////////////////////////////////////////////////////////////


#ifndef _ACELOG_H_
#define _ACELOG_H_

#include "ace/Task.h"
#include "ace/Reactor.h"

#define DEBUG_PREFIX       ACE_TEXT ("[%D][%P-%t][%M] %N(%l):\n")
#define HRJ_DEBUG_PX  LM_DEBUG, DEBUG_PREFIX
#define HRJ_ERROR_PX  LM_ERROR, DEBUG_PREFIX

class CLogMonitor :public ACE_Task<ACE_MT_SYNCH>
{
public:
    ACE_INT32 run(void)
    {
         return activate(THR_NEW_LWP,1);//新建一个线程

    }
protected:
    virtual int svc(void)
    {
        ACE_Reactor::instance()->owner(ACE_Thread::self());
        ACE_Reactor::instance()->run_reactor_event_loop();
        return 0;
    }
};

#endif