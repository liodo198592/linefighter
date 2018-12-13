#include "status_comm.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif
#include "model_comm.h"
#ifdef LF_COCOS 
#include "channel_cocos.h"
#else
#include "channel.h"
#endif
//#include "conio.h"
#include "stdio.h"

extern char g_Control1Name[30];
extern int g_iTimeOut;
extern CCmdStatus * pMystatus;
extern CCmdStatus * pMystatus_old;
extern char g_readString[300];

string CClientStatusRoom::getTitleInfo(string username)
{
	char content[200] = {0};
	sprintf(content,"%s �Ѿ������˷��� %s  ����ID = %d ׼���У� �����ĵȴ�������ҿ�ʼ",username.c_str(), room.getRoomName().c_str(),room.getRoomID());
	string subStr(content);
	return subStr;
}

void CClientStatusRoom::showInfo()
{
	printf("%s �Ѿ������˷��� %s  ����ID = %d\n",g_Control1Name, room.getRoomName().c_str(),room.getRoomID());
	
	//��ȡ�����û��б�
	if (getRoomUser(room.getRoomID()) != RTN_SUCCESS)
	{
		printf("��ȡ�û��б�ʧ��",g_Control1Name);
	}
	else
	{
		showRoomUser();
	}
	
	showChatQueue();

	printf("****ϵͳ��Ϣ*****�� %s\n",lastCommandResult.c_str());
	lastCommandResult = "";

}

void CClientStatusRoom::showMenu()
{
	int menuID;
	int roomID;
	char ch = 0;

	if (getMyStatus() == CUSER_READY)
	{
		printf("׼���У� �����ĵȴ�������ҿ�ʼ\n");
		OSTimeDly(2);
		return;
	}
	if (getMyStatus() == CUSER_GAMMING)
	{
		pMystatus_old = pMystatus;
		pMystatus = new CClientStatusGame(room,userName);
		return;
	}
	
	printf("1 : ׼����ʼ��������Ҷ�׼������Ϸ��ʼ,���˿�ʼ�Ļ����Զ�����AI���ԣ�\n");
	printf("2 : �˳����� \n");
	printf("3 : ����������Ϣ \n");
	printf("5 : ˢ�� \n");
	//key_getMenuButten(menuID,2);
	getchar();
	scanf("%d",&menuID);
	if (menuID == 1)
	{
		if(readytoStart(room.getRoomID()) != RTN_SUCCESS)
		{
			lastCommandResult = "׼����ʼ��Ϸʧ��";
		}
	}
	if (menuID == 3)
	{
		memset(g_readString,0,300);
		getchar();
		printf("������������Ϣ�����Ȳ��ܴ���200 \n");
		
		//key_getString(content,50);
		getchar();
		scanf("%s",&g_readString);
		string content(g_readString);
		string userName(g_Control1Name);
		CChatInfo cChatInfo;
		cChatInfo.setcontent(content);
		cChatInfo.setsenderNamee(userName);
		SDate date;
		int iNow = OSgetNowSeconds();
		TimeToDate(date,iNow);
		cChatInfo.settime(date);

		if(sendChartInfo(room.getRoomID(),cChatInfo) == RTN_SUCCESS)
		{
			lastCommandResult = "����������Ϣ�ɹ�";
		}
	}
	else if (menuID == 2)
	{
		if(quitRoom(room.getRoomID()) != RTN_SUCCESS)
		{
			lastCommandResult = "�뿪����ʧ��";
		}
		else
		{
			pMystatus_old = pMystatus;
			pMystatus = new CClientStatusHouse();
		}
	}
}

int CClientStatusRoom::getRoomUser(int roomID)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskUserinRoom(g_Control1Name,roomID,buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(PLAYER_IN_ROOM_ASW,buf,len) == RTN_SUCCESS)
	{
		vecUserInRoom.clear();
		if (getProtocol()->ptclCheckAskUserinRoomAsw(vecUserInRoom,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("���뷿��ʧ��\n");
	return RTN_ERROR;
}

string CClientStatusRoom::getRoomUserInfo(int index)
{
	string usrName = "";
	string usrReady = "";
	char content[200] = {0};
	
	
	if (index < vecUserInRoom.size())
	{
		usrName = vecUserInRoom[index].getUserName();
		if (vecUserInRoom[index].getReady() == CUSER_READY)
		{
			usrReady = "(׼����)";
		}
		else if (vecUserInRoom[index].getReady() == CUSER_GAMMING)
		{
			usrReady = "(��Ϸ��)";
		}
		else
		{
			usrReady = "(δ׼��)";
		}
	}
	else
	{
		usrName = "δ�����";
		usrReady = "";
	}
	sprintf(content,"$λ�� %d $ �� %s %s",index,usrName.c_str(),usrReady.c_str());
	string substring(content);
	return substring;
}

int CClientStatusRoom::showRoomUser()
{
	string usrName = "";
	string usrReady = "";
	for(int i = 0; i < 4 ; ++i)
	{
		if (i < vecUserInRoom.size())
		{
			usrName = vecUserInRoom[i].getUserName();
			if (vecUserInRoom[i].getReady() == CUSER_READY)
			{
				usrReady = "(׼����)";
			}
			else if (vecUserInRoom[i].getReady() == CUSER_GAMMING)
			{
				usrReady = "(��Ϸ��)";
			}
			else
			{
				usrReady = "(δ׼��)";
			}
		}
		else
		{
			usrName = "δ�����";
			usrReady = "";
		}
		printf("$λ�� %d $ �� %s %s \n",i,usrName.c_str(),usrReady.c_str());
	}
	return RTN_SUCCESS;
}

string CClientStatusRoom::getChatInfo()
{
	char info[2000];
	string strInfo = "";
	int size = qChatqueue.size();
	for (int i = 0 ; i < size ; ++i)
	{
		CChatInfo cChatInfo = qChatqueue.front();
		qChatqueue.pop();
		qChatqueue.push(cChatInfo);
		SDate date = cChatInfo.gettime();
		sprintf(info,"%s �� %02d:%02d:%02d %02d:%02d:%02d ˵�� %s \n",cChatInfo.getsenderName().c_str(),
			date.year,date.month,date.day,date.hour,date.min,date.sec,cChatInfo.getcontent().c_str());
		string substring(info);
		strInfo += substring;
	}
	return strInfo;
}

int CClientStatusRoom::showChatQueue()
{
	printf("////////////////////////////////////CHAT INFO///////////////////////\n");
	int size = qChatqueue.size();
	for (int i = 0 ; i < size ; ++i)
	{
		CChatInfo cChatInfo = qChatqueue.front();
		qChatqueue.pop();
		qChatqueue.push(cChatInfo);
		SDate date = cChatInfo.gettime();
		printf("%s �� %02d:%02d:%02d %02d:%02d:%02d ˵�� %s \n",cChatInfo.getsenderName().c_str(),
			date.year,date.month,date.day,date.hour,date.min,date.sec,cChatInfo.getcontent().c_str());
	}
	printf("/////////////////////////////////////////////////////////////////////\n");
	return 0;
}

int CClientStatusRoom::sendChartInfo(int roomID , CChatInfo cChatInfo)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildChatInfo(g_Control1Name,roomID,cChatInfo,buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
		return RTN_ERROR;
	}
	return RTN_SUCCESS;	
}

int CClientStatusRoom::quitRoom(int roomID)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildLeaveRoom(g_Control1Name,roomID,buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(LEAVE_ROOM_ASW,buf,len) == RTN_SUCCESS)
	{
		if (getProtocol()->ptclCheckLeaveRoomAsw(buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("�뿪����ʧ��\n");
	return RTN_ERROR;
}

int CClientStatusRoom::readytoStart(int roomID)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildReadyStart(g_Control1Name,roomID,buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(READY_START_ASW,buf,len) == RTN_SUCCESS)
	{
		if (getProtocol()->ptclCheckReadyStartAsw(buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("׼����ʼʧ��\n");
	return RTN_ERROR;
}

int CClientStatusRoom::getMyStatus()
{
	for (vector<CUser>::iterator it = vecUserInRoom.begin(); it != vecUserInRoom.end() ; ++it)
	{
		string strName = (*it).getUserName();
		if (strcmp(strName.c_str(),userName.c_str()) == 0)
		{
			return (*it).getReady();
		}
	}
	return RTN_ERROR;
}