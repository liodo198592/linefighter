#ifdef LF_COCOS 
#include "channel_cocos.h"
#else
#include "channel.h"
#endif
#ifdef MAKE_ANDRIOD
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock2.h>
#endif
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
		g_pConn->Ctcpclose();
		g_pConn->Ctcpopen();
		if(!g_pConn->isOpen())
		{
			printf("connect error!\n");
			OSTimeDly(1);
		}
		else
		{
			//g_pConn->m_bOpen = true;
		}
	}
	return g_pConn;
}

int CTCPClient::CtcpSend(const unsigned char buf[], int len)
{
	int ret = send(m_peer,(const char *) buf, len, 0);
	if (ret == -1)
	{
		//printf("send() failed: %dn", WSAGetLastError());
		return RTN_ERROR;
	}
	return RTN_SUCCESS;
}

int CTCPClient::CtcpReceive(unsigned char buf[], size_t len, float timeOut,size_t * bytes_transferred)
{
  int iResult = 0;

  iResult = recv(m_peer, (char *)buf, len, 0);
  if(iResult > 0)
  {
	  *bytes_transferred = iResult;
  }
  
  return iResult;
}

string CTCPClient::Ctcpopen()
{
#ifdef MAKE_ANDRIOD

	struct sockaddr_in sa;
	struct hostent* hp;

	hp = gethostbyname(m_sIP.c_str());
	if(!hp){
		return "";
	}
	memset(&sa, 0, sizeof(sa));
	memcpy((char*)&sa.sin_addr, hp->h_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(m_iPort);

	m_peer = socket(sa.sin_family, SOCK_STREAM, 0);

	if(m_peer < 0){
		printf( "failed to create socket\n" );
		return "";
	}
	if(::connect(m_peer, (sockaddr*)&sa, sizeof(sa)) < 0){
		printf( "failed to connect socket\n" );
		::close(m_peer);
		return "";
	}
#else // MAKE_ANDRIOD

	WSADATA wsd;
	WSACleanup();
	if(WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		//printf;
		return "Failed to load Winsock library!n";
	}
	m_peer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_peer == INVALID_SOCKET)
	{
		//printf(");
        return "socket() failed";
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_iPort);
	addr.sin_addr.s_addr = inet_addr(m_sIP.c_str());


	if (connect(m_peer, (const sockaddr *)&addr ,sizeof(addr)) == SOCKET_ERROR)
	{
		//printf("connect() failed: %dn", WSAGetLastError());
		return "";
	}
#endif

	this->m_bOpen = true;
	return "Connect successfully!";
}

void CTCPClient::Ctcpclose()
{
   int iResult = 0;
#ifdef MAKE_ANDRIOD
   if((iResult=close(m_peer)) == 0)
	   m_bOpen = false;
#else
   if((iResult=closesocket(m_peer)) == 0)
	   m_bOpen = false;
#endif // MAKE_ANDRIOD

   
}