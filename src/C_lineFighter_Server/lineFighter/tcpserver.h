#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "global.h"
class CTCPServer;
extern void (*pFuncPoint)(ACE_SOCK_STREAM stream, ACE_Task<ACE_MT_SYNCH> *& task,CTCPServer& server);

//tcp·þÎñÆ÷Àà
class CTCPServer
{	
public:
	CTCPServer(string sIP,unsigned short iPort, int iMaxConnection=100): m_sIP(sIP),m_iPort(iPort),
		                                                                m_iMaxConnection(iMaxConnection),m_iCount(0){}
	virtual ~CTCPServer(){m_acceptor.close();}
	int open();
	int accept(void (*pFuncPoint)(ACE_SOCK_STREAM stream, ACE_Task<ACE_MT_SYNCH> *& task, CTCPServer* server));
	int close(ACE_SOCK_STREAM& peer);

private:
	ACE_SOCK_Acceptor m_acceptor;
	int m_iMaxConnection;
	int m_iCount;
	string m_sIP;
	unsigned short m_iPort;
    
};

#endif
