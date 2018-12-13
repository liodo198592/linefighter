#include "tcpserver.h"
#include "CEngine.h"

//extern vector<CEngine *> vConnectionList;
//extern ACE_Thread_Mutex mutex_vConnectionList;

int CTCPServer::open()
{
	ACE_INET_Addr addr(m_iPort, m_sIP.c_str());
	return m_acceptor.open(addr, 1);	
}

int CTCPServer::accept(void (*pFuncPoint)(ACE_SOCK_Stream stream, ACE_Task<ACE_MT_SYNCH> *& task,CTCPServer* server))
{
	while(true)
	{
		ACE_SOCK_Stream peer;
		if(m_acceptor.accept(peer) == -1)
			return -1;
		else
		{
			if (m_iCount < m_iMaxConnection)
			{
                ++m_iCount;

				ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("CTCPServer::accept():establish connection.CONN COUNT]:%d\n"),m_iCount));
				
				ACE_Task<ACE_MT_SYNCH> * pTask;
				pFuncPoint(peer, pTask, this);
				pTask->activate(THR_NEW_LWP|THR_DETACHED,1);
                
                {
                    //ACE_Guard<ACE_Thread_Mutex> guard(mutex_vConnectionList);
                    //vConnectionList.push_back((CEngine *)pTask);
                }
                
			}
			else peer.close();
		}
	}
	return 0;
}

int CTCPServer::close(ACE_SOCK_STREAM& peer)
{
    char addr[100];
    int retCode = 0;
    ACE_INET_Addr remoteAddr;
    peer.get_remote_addr(remoteAddr);
    remoteAddr.addr_to_string(addr,sizeof(addr));
#ifdef WIN32
    peer.close_writer();
#endif
	retCode = peer.close();
    --m_iCount;

    ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("CTCPServer::close():close connection."
                                      "[ADDR&PORT]:%s,[HOSTNAME]:%s,[RETCODE]:%d,[CONN COUNT]:%d\n"), 
                                      addr,remoteAddr.get_host_name(),retCode,m_iCount));
	return 0;
}
