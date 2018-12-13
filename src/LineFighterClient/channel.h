#ifndef CHANNEL_H
#define CHANNEL_H

#include "global.h"



class CChannel
{
public:
	CChannel():m_bOpen(false),m_bAcquired(false){}
	virtual ~CChannel(){}

	virtual int send(const unsigned char buf[], int len)=0;
	virtual int receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut=0,size_t * bytes_transferred=0)=0;
	virtual string open()=0;
	virtual void close()=0;

	bool isOpen() { return m_bOpen;}
	bool isAcquired() { return m_bAcquired;}
	void setAcquired() {m_bAcquired = true;}

protected:
	bool m_bOpen;
	bool m_bAcquired;

};


class CTCPClient: public CChannel
{
public:
	  CTCPClient(string sIP,unsigned short iPort,int time_out = 2):m_sIP(sIP),m_iPort(iPort),m_timeout(time_out){}
	  virtual ~CTCPClient(){}
	  virtual int send(const unsigned char buf[], int len);
	  virtual int receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut=0,size_t * bytes_transferred=0);
	  virtual string open();
	  virtual void close();

public:
	  unsigned short m_iPort;
	  string m_sIP;
	  ACE_SOCK_Stream m_peer;

	  const ACE_Time_Value m_timeout;
};


//class CTSingHuaClient: public CTCPClient
//{
//public:
//	CTSingHuaClient(string sIP,unsigned short iPort,string sName,string sPassword,int time_out = 2):CTCPClient(sIP,iPort,time_out),
//		                                                                                            m_sName(sName),
//																									m_sPassword(sPassword),
//																									m_iRepID(0){}
//	virtual ~CTSingHuaClient(){}
//	virtual int send(const unsigned char buf[], int len);
//	virtual int receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut=0,size_t * bytes_transferred=0);
//	virtual string open();
//	virtual void close();
//
//private:
//	string m_sName;
//	string m_sPassword;
//	ACE_INT32 m_iRepID;
//};


class CCom : public CTCPClient//认为串口是一种TCP连接，复用
{
public:
	CCom(int comnum,int baudrate,int databits,int stopbits,int parityenb,int time_out = 2):CTCPClient("",0,time_out),
															_comnum(comnum),_baudrate(baudrate),_databits(databits),_stopbits(stopbits),_parityenb(parityenb){}
	~CCom(){};
	virtual string open();
	virtual int send(const unsigned char buf[], int len);
	virtual int receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut=0,size_t * bytes_transferred=0);
	virtual void close();

protected:
	int _comnum;//串口号
	int _baudrate;//波特率
	int _databits;//数据位
	int _stopbits;//停止位
	int _parityenb;//效验位
	int li_ret;
	ACE_TTY_IO l_tty;

};

//class CPKUClient: public CTCPClient
//{
//public:
//	CPKUClient(string sIP,unsigned short iPort,int time_out = 2):CTCPClient(sIP,iPort,time_out){}
//	virtual ~CPKUClient(){}
//};

#endif
