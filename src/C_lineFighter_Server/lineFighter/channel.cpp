#include "channel.h"

//////////////////////////////////////////////////////////////////////////

CTCPClient * g_pConn = NULL;

CTCPClient* getConn()
{
	if (g_pConn == NULL)
	{
		g_pConn = new CTCPClient("127.0.0.1",10091);
	}
	if (!g_pConn->isOpen())
	{
		g_pConn->close();
		g_pConn->open();
		if(!g_pConn->isOpen())
		{
			
			OSTimeDly(1);
		}
		else
		{
			
		}
	}
	return g_pConn;
}

int CTCPClient::send(const unsigned char buf[], int len)
{
  return m_peer.send((void*)buf, len);
}

int CTCPClient::CtcpReceive(unsigned char buf[], size_t len,float timeOut,size_t * bytes_transferred)
{
  int iResult = 0;
  time_t t = timeOut;
  ACE_Time_Value time(t,0);

  if((iResult = m_peer.recv_n(buf, len, (const ACE_Time_Value *)&time,bytes_transferred)) == 0) close();
  
  return iResult;
}

string CTCPClient::open()
{
	printf("connecting %s - %d \n",m_sIP.c_str(),m_iPort);
	ACE_SOCK_Connector connector;
	ACE_INET_Addr server(m_iPort,m_sIP.c_str());
	ACE_Time_Value time(m_timeout,0);
	if(-1 == connector.connect(m_peer, server, (const ACE_Time_Value *)&time)) 
	{
		printf("Connect fail! \n");
		return "Connect fail!";
	}
	m_bOpen = true;

	printf("Connect successfully! \n");
	return "Connect successfully!";
}

void CTCPClient::close()
{
   int iResult = 0;

   if((iResult=m_peer.close()) == 0)
	   m_bOpen = false;
}