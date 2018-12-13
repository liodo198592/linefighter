#include "status_comm.h"
#include "CProtocol.h"
#ifdef LF_COCOS 
#include "channel_cocos.h"
#else
#include "channel.h"
#endif

extern char g_Control1Name[30];
extern int g_iTimeOut;
extern CCmdStatus * pMystatus;
extern CCmdStatus * pMystatus_old;

void CClientStatusLogIn::showInfo()
{
	printf("��ӭ���� LF 1.0\n");
	printf("�������û��� , ���Ӣ����!\n");
}

void CClientStatusLogIn::showMenu()
{
	int ret = RTN_SUCCESS;
	string usrName;
	memset(g_Control1Name,0,sizeof(g_Control1Name));
	getchar();
	scanf("%s",g_Control1Name);
	
	/*if (key_getString(usrName,5) != RTN_SUCCESS)
	{
		return;
	}*/
	memcpy(g_Control1Name,usrName.c_str(),strlen(usrName.c_str()));

	unsigned char buf[2000];
	int len = getProtocol()->ptclBuildLogin(g_Control1Name,buf);
	
	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	
	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(LOGIN_ASW,buf,len) == RTN_SUCCESS)
	{
		//����½�Ƿ�ɹ�
		if (getProtocol()->ptclCheckLogin(buf,len) == RTN_SUCCESS)
		{
			pMystatus_old = pMystatus;
			pMystatus = new CClientStatusHouse();
		}
	}
	printf("��½ʧ��\n");
	return ;
}

int CClientStatusLogIn::login(string strUsrName)
{
	int ret = 0;
	memcpy(g_Control1Name,strUsrName.c_str(),strlen(strUsrName.c_str()));

	unsigned char buf[2000];
	int len = getProtocol()->ptclBuildLogin(g_Control1Name,buf);
	
	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret < RTN_SUCCESS)
	{
		return RTN_ERROR;
	}
	
	memset(buf,0,sizeof(buf));
	if(recvAReportFormServer(LOGIN_ASW,buf,len) == RTN_SUCCESS)
	{
		//����½�Ƿ�ɹ�
		if (getProtocol()->ptclCheckLogin(buf,len) == RTN_SUCCESS)
		{
			return RTN_SUCCESS;
		}
	}
	return RTN_ERROR;
}
