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
	sprintf(content,"%s 已经进入了房间 %s  房间ID = %d 准备中： 请耐心等待其他玩家开始",username.c_str(), room.getRoomName().c_str(),room.getRoomID());
	string subStr(content);
	return subStr;
}

void CClientStatusRoom::showInfo()
{
	printf("%s 已经进入了房间 %s  房间ID = %d\n",g_Control1Name, room.getRoomName().c_str(),room.getRoomID());
	
	//获取在线用户列表
	if (getRoomUser(room.getRoomID()) != RTN_SUCCESS)
	{
		printf("获取用户列表失败",g_Control1Name);
	}
	else
	{
		showRoomUser();
	}
	
	showChatQueue();

	printf("****系统信息*****： %s\n",lastCommandResult.c_str());
	lastCommandResult = "";

}

void CClientStatusRoom::showMenu()
{
	int menuID;
	int roomID;
	char ch = 0;

	if (getMyStatus() == CUSER_READY)
	{
		printf("准备中： 请耐心等待其他玩家开始\n");
		OSTimeDly(2);
		return;
	}
	if (getMyStatus() == CUSER_GAMMING)
	{
		pMystatus_old = pMystatus;
		pMystatus = new CClientStatusGame(room,userName);
		return;
	}
	
	printf("1 : 准备开始（所有玩家都准备则游戏开始,单人开始的话会自动补充AI电脑）\n");
	printf("2 : 退出房间 \n");
	printf("3 : 发送聊天信息 \n");
	printf("5 : 刷新 \n");
	//key_getMenuButten(menuID,2);
	getchar();
	scanf("%d",&menuID);
	if (menuID == 1)
	{
		if(readytoStart(room.getRoomID()) != RTN_SUCCESS)
		{
			lastCommandResult = "准备开始游戏失败";
		}
	}
	if (menuID == 3)
	{
		memset(g_readString,0,300);
		getchar();
		printf("请输入聊天信息，长度不能大于200 \n");
		
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
			lastCommandResult = "发送聊天信息成功";
		}
	}
	else if (menuID == 2)
	{
		if(quitRoom(room.getRoomID()) != RTN_SUCCESS)
		{
			lastCommandResult = "离开房间失败";
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

	//发送报文
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
	printf("加入房间失败\n");
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
			usrReady = "(准备中)";
		}
		else if (vecUserInRoom[index].getReady() == CUSER_GAMMING)
		{
			usrReady = "(游戏中)";
		}
		else
		{
			usrReady = "(未准备)";
		}
	}
	else
	{
		usrName = "未加入的";
		usrReady = "";
	}
	sprintf(content,"$位置 %d $ ： %s %s",index,usrName.c_str(),usrReady.c_str());
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
				usrReady = "(准备中)";
			}
			else if (vecUserInRoom[i].getReady() == CUSER_GAMMING)
			{
				usrReady = "(游戏中)";
			}
			else
			{
				usrReady = "(未准备)";
			}
		}
		else
		{
			usrName = "未加入的";
			usrReady = "";
		}
		printf("$位置 %d $ ： %s %s \n",i,usrName.c_str(),usrReady.c_str());
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
		sprintf(info,"%s 在 %02d:%02d:%02d %02d:%02d:%02d 说： %s \n",cChatInfo.getsenderName().c_str(),
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
		printf("%s 在 %02d:%02d:%02d %02d:%02d:%02d 说： %s \n",cChatInfo.getsenderName().c_str(),
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

	//发送报文
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

	//发送报文
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
	printf("离开房间失败\n");
	return RTN_ERROR;
}

int CClientStatusRoom::readytoStart(int roomID)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildReadyStart(g_Control1Name,roomID,buf);

	//发送报文
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
	printf("准备开始失败\n");
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