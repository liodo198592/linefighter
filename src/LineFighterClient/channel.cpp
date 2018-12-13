#include "channel.h"


int CTCPClient::send(const unsigned char buf[], int len)
{
  return m_peer.send((void*)buf, len);
}

int CTCPClient::receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut,size_t * bytes_transferred)
{
  int iResult = 0;

  if((iResult = m_peer.recv(buf, len, timeOut)) == 0) close();

  return iResult;
}

string CTCPClient::open()
{
	//ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("CMsgHandler::svc():Connecting..网络/GATEWAY抄收方式－----\n.[IP]: %s, [PORT]: %d.\n"),m_sIP.c_str(), m_iPort));
	ACE_SOCK_Connector connector;
	ACE_INET_Addr server(m_iPort,m_sIP.c_str());	
	if(-1 == connector.connect(m_peer, server, &m_timeout)) return "Connect fail!";
	m_bOpen = true;
	ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("CMsgHandler::svc():Connection established![IP]: %s, [PORT]: %d\n"),m_sIP.c_str(),m_iPort));
	return "Connect successfully!";
	
}

void CTCPClient::close()
{
   if(m_peer.close() == 0)
	   m_bOpen = false;
}

//////////////////////////////////////////////////////////////////////////Com
string CCom::open()
{
	ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("CMsgHandler::svc():Connecting..串口抄收方式－---\n串口号[%d].波特率[%d] 数据位[%d] 停止位[%d] 校验位[%d].\n"),
		_comnum,_baudrate,_databits,_stopbits,_parityenb));
	ACE_HANDLE l_term;
	switch(_comnum)
	{
	case 1:
		l_term = ACE_OS::open(PORT_ADDR_1, O_RDWR );
		break;
	case 2:
		l_term = ACE_OS::open(PORT_ADDR_2, O_RDWR );
		break;
	case 3:
		l_term = ACE_OS::open(PORT_ADDR_3, O_RDWR );
		break;
	case 4:
		l_term = ACE_OS::open(PORT_ADDR_4, O_RDWR );
		break;
	case 5:
		l_term = ACE_OS::open(PORT_ADDR_5, O_RDWR );
		break;
	case 6:
		l_term = ACE_OS::open(PORT_ADDR_6, O_RDWR );
		break;
	case 7:
		l_term = ACE_OS::open(PORT_ADDR_7, O_RDWR );
		break;
	case 8:
		l_term = ACE_OS::open(PORT_ADDR_8, O_RDWR );
		break;
	case 9:
		l_term = ACE_OS::open(PORT_ADDR_9,O_RDWR);
		break;
	case 10:
		l_term = ACE_OS::open(PORT_ADDR_10,O_RDWR);
		break;
	case 11:
		l_term = ACE_OS::open(PORT_ADDR_11,O_RDWR);
		break;
	case 12:
		l_term = ACE_OS::open(PORT_ADDR_12,O_RDWR);
		break;
	default:
		l_term = ACE_OS::open(PORT_ADDR_1, O_RDWR );

	}
	if (l_term == ACE_INVALID_HANDLE) 
	{
		ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("打开COM端口失败 ! ")));
		return "打开COM端口失败!" ;
	}

	ACE_TTY_IO::Serial_Params l_sp;
	memset (&l_sp,0,sizeof(l_sp));

	l_sp.baudrate          = _baudrate;
	l_sp.databits          = _databits;
	l_sp.stopbits          = _stopbits;
#ifdef WIN32
	l_sp.parityenb         = _parityenb;
#endif
	l_sp.modem             = 0;      //是否是调制解调器
	l_sp.rcvenb            = 1;      //是否可接收

	l_sp.readmincharacters = 4;
	l_sp.readtimeoutmsec   = 500;

	ACE_OS::fcntl(l_term,SO_SNDBUF,REPORT_LENGTH);
	ACE_OS::fcntl(l_term,SO_RCVBUF,REPORT_LENGTH);

	l_tty.set_handle(l_term);
	l_tty.disable(ACE_SIGIO);
	l_tty.disable(ACE_SIGURG);
	li_ret=l_tty.enable(ACE_NONBLOCK);
	li_ret=l_tty.control (ACE_TTY_IO::SETPARAMS ,&l_sp);   
	m_bOpen = true;

	ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("CMsgHandler::svc():Connecting..串口抄收方式－---连接成功 ！ \n串口号[%d].波特率[%d] 数据位[%d] 停止位[%d] 校验位[%d].\n"),
		_comnum,_baudrate,_databits,_stopbits,_parityenb));
	return "COM Connect successfully!";
}

int CCom::send(const unsigned char * buf, int len)
 {
	 return l_tty.send(buf, len);
 }

int CCom::receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut,size_t * bytes_transferred)
{
	size_t nbytes = -1;
	li_ret = l_tty.recv_n(buf,len,timeOut,&nbytes);
#ifdef WIN32
	if (li_ret == -1) return -1;
	if (li_ret == 0)
	{
		if (nbytes == 0)
		{
			ACE_OS::last_error(ETIME);
			return -1;
		}
		else if(nbytes > 0) return nbytes;
		else return 0;
	}
	return 0;
#else 
	return nbytes;
#endif
}

void CCom::close()
{
	if (l_tty.close())
	{
		m_bOpen = false;
	}
}

///////////////////////////////////////////清华思路////////////////////////////////////////////////////
//int CTSingHuaClient::send(const unsigned char buf[], int len)
//{
//	char repID[128];
//	char num[16];
//	m_iRepID = (m_iRepID%10000)+1;
//	ACE_OS::itoa(m_iRepID,num,10);
//	sprintf(repID,"hrj6000%s",num);
//
//	char destAddr[64] = {0};
//	if(buf[0]==0x68 && buf[7]==0x68) 
//	{
//		for(ACE_INT32 i=1; i <= 2; ++i) 
//			sprintf(destAddr+strlen(destAddr),"%02x",buf[i]);
//		for(ACE_INT32 i=4; i >= 3; --i) 
//			sprintf(destAddr+strlen(destAddr),"%02x",buf[i]);
//	}
//	
//	size_t iLen = 0;
//	unsigned char * base64Buf = NULL;
//	if ((base64Buf = ACE_Base64::encode((unsigned char *)buf,len,&iLen))==NULL || iLen==0)
//	{
//		ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("CTSingHuaClient::send():base64 encode error.")));
//		return -1;
//	}
//
//	string sendXML = "<UMSData><RepID>"+string(repID)+"</RepID><ErrCode>0</ErrCode><ErrReason></ErrReason>"
//	"<DestAddress><Address outway=\"GPRS\" type=\"TCP\" ipaddress=\"10.136.188.94\" ipport=\"6666\">"+destAddr+"</Address></DestAddress>"
//	"<SrcAddress><Address inway=\"Client\">2008</Address></SrcAddress>"
//	"<Notify import=\"0\"><Message><Segment coding=\"4\"><Label></Label><Value>"+string((char*)base64Buf)+"</Value></Segment></Message></Notify></UMSData>";
//	
//			
//	if (SubmitEx((char *)sendXML.c_str(),(ACE_INT32)sendXML.size()) != SAT_OK) 
//	{
//		delete[] base64Buf;
//		return -1;
//	}
//
//	delete[] base64Buf;
//	return 0;
//}
//
////-1：未收到数据（异常或无数据或非二进制数据）
////0：连接断开
////其他：返回长度（收到数据）
//int CTSingHuaClient::receive(unsigned char buf[], size_t len, const ACE_Time_Value * timeOut,size_t * bytes_transferred)
//{
//	char msgXml[1024*8];
//	char errCode[8];
//	char errReason[256];
//	char srcType[8];
//	char srcAddr[16];
//	ACE_INT32 coding = 0;
//	ACE_INT32 iRecvLen = 0;
//
//	ACE_INT32 iResult = PeekMsg(msgXml,errCode,errReason,srcType,srcAddr,(char *)buf,&iRecvLen,&coding);
//
//	if( iResult != SAT_OK &&
//		iResult != SAT_NORECVMSG) 
//	{
//		if (MoTestActive() != SAT_OK || 
//			MtTestActive() != SAT_OK) 
//		{
//			close();
//			return 0;
//		}
//	}
//
//	if (iRecvLen > 0 && coding == 4) 
//		return iRecvLen;
//
//	return -1;
//}
//
//string CTSingHuaClient::open()
//{	
//	if (MtLogin((char *)m_sIP.c_str(),m_iPort,(char *)m_sName.c_str(),(char *)m_sPassword.c_str()) != SAT_OK ||
//		MoLogin((char *)m_sIP.c_str(),m_iPort,(char *)m_sName.c_str(),(char *)m_sPassword.c_str()) != SAT_OK)
//		return "Connect fail!";
//
//	m_bOpen = true;
//	return "Connect successfully!";
//}
//
//void CTSingHuaClient::close()
//{
//	if (MoLogOut() == SAT_OK && MtLogOut() == SAT_OK) m_bOpen = false;
//}