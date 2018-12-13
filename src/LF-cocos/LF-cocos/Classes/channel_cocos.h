#ifndef CHANNEL_H
#define CHANNEL_H

#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

#ifdef MAKE_ANDRIOD
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif

class CChannel
{
public:
	CChannel():m_bOpen(false),m_bAcquired(false),m_bFcb(0x20){}
	virtual ~CChannel(){}

	virtual int CtcpSend(const unsigned char buf[], int len)=0;
	virtual int CtcpReceive(unsigned char buf[], size_t len, float timeOut=0,size_t * bytes_transferred=0)=0;
	virtual string Ctcpopen()=0;
	virtual void Ctcpclose()=0;

	bool isOpen() { return m_bOpen;}
	bool isAcquired() { return m_bAcquired;}
	unsigned char fcb(){return m_bFcb;}
	void fcb(unsigned char iFcb){ m_bFcb = iFcb;}
	void setAcquired(bool state = true) {m_bAcquired = state;}

protected:
	bool m_bOpen;
	bool m_bAcquired;
	unsigned char m_bFcb;
};



//Õ¯¬ÁÕ®µ¿¿‡
class CTCPClient: public CChannel
{
public:
	  CTCPClient(string sIP,unsigned short iPort,int time_out = 2):m_sIP(sIP),m_iPort(iPort),m_timeout(time_out){}
	  virtual ~CTCPClient(){}
	  virtual int CtcpSend(const unsigned char buf[], int len);
	  virtual int CtcpReceive(unsigned char buf[], size_t len, float timeOut=0,size_t * bytes_transferred=0);
	  virtual string Ctcpopen();
	  virtual void Ctcpclose();

public:
	  unsigned short m_iPort;
	  string m_sIP;
#ifdef MAKE_ANDRIOD
	  int m_peer;
#else
	  SOCKET m_peer;
#endif
	  const float m_timeout;
};

CTCPClient* getConn();

#endif
