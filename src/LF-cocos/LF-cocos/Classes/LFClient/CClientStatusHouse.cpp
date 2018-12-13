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


extern char g_Control1Name[30];
extern int g_iTimeOut;
extern CCmdStatus * pMystatus;
extern CCmdStatus * pMystatus_old;
char g_readString[300];


string CClientStatusHouse::getTitleInfo()
{
	char content[250] = {0};
	sprintf(content,"%s已经进入了大厅",g_Control1Name);
	string retStr(content);
	return retStr;
}

string CClientStatusHouse::getOneUserStr(int index)
{
	int count = 0;
	if (m_UserList.size() < index)
	{
		return "";
	}
	for (map<string,CUser>::iterator it = m_UserList.begin(); it != m_UserList.end(); it++)
	{
		if (count == index)
		{
			string content = "";
			content += "$";
			content += it->first;
			content += "$\n";
			return content;
		}
		count++;
	}
	return "";
}

string CClientStatusHouse::getUserInfoStr()
{
	string content = "";
	int i = 1;
	char num[20] = {0};
	sprintf(num,"%02d",m_UserList.size());
	string numstr(num);
	content += "当前在线用户(";
	content +=  numstr;
	content += "位)：";
	return content;
}

string CClientStatusHouse::getRoomTotalInfoStr()
{
	string content = "";
	int i = 1;
	char num[20] = {0};
	sprintf(num,"%02d",m_roomList.size());
	string numstr(num);
	content += "当前房间(";
	content +=  numstr;
	content += "间)：";
	return content;
}

string CClientStatusHouse::getRoomInfoStr(int index , int& RoomID)
{
	char content[512] = {0};
	int count = 0;
	for(map<int,CRoom>::iterator it = m_roomList.begin(); it !=  m_roomList.end(); ++it)
	{
		if (count == index)
		{
			string roomstat ="";
			if (it->second.getRoomStatus() == EROOM_STATUS_READY)
			{
				roomstat = "准备中";
			}
			else if (it->second.getRoomStatus() == EROOM_STATUS_GAMING)
			{
				roomstat = "游戏中";
			}
			else
			{
				roomstat = "已结束";
			}

		/*	sprintf(content,"$房间ID = %d ， 房间名称 = %s  创建者 = %s 房间人数 %d/%d 房间状态 %s $"
				,it->second.getRoomID(),it->second.getRoomName().c_str(),it->second.gethostUserName().c_str()
				,it->second.getnowPlayer(),it->second.getmaxPlayer(),roomstat.c_str());*/
			sprintf(content,"$房间名称 = %s,房间人数 %d/%d 房间状态 %s $"
				,it->second.getRoomName().c_str(),it->second.getnowPlayer(),it->second.getmaxPlayer(),roomstat.c_str());
			string strsub(content);
			RoomID = it->first;
			return strsub;
		}
		else
		{
			count++;
		}
	}
	return "";
}

void CClientStatusHouse::showInfo()
{
	printf("%s已经进入了大厅\n",g_Control1Name);

	//获取在线用户列表
	if (getOnlneList() != RTN_SUCCESS)
	{
		printf("获取用户列表失败",g_Control1Name);
	}
	else
	{
		showOnlineList();
	}
	//获取房间列表
	if (getRoomList() != RTN_SUCCESS)
	{
		printf("获取房间列表失败",g_Control1Name);
	}
	else
	{
		showRoomList();
	}
	printf("****系统信息*****： %s\n",lastCommandResult.c_str());
	lastCommandResult = "";
}

void CClientStatusHouse::showMenu()
{
	int menuID = 0;
	int roomID;
	
	printf("1 : 创建房间 \n");
	printf("2 : 加入房间 \n");
	printf("3 : 退出 \n");
	printf("5 : 刷新 \n");
	getchar();
	scanf("%d",&menuID);
	//key_getMenuButten(menuID,2);
	if (menuID == 3)
	{
		pMystatus_old = pMystatus;
		pMystatus = new CClientStatusLogIn();
	}
	if (menuID == 1)
	{
		memset(g_readString,0,30);	
		printf("请输入房间名称 （人数默认为4，人数不足开始游戏的话会补AI） \n");
		//key_getString(roomName,5);
		getchar();
		scanf("%s",&g_readString);
		string roomName(g_readString);
		CRoom room;
		if(createRoom(room,roomName) == RTN_SUCCESS)
		{
			pMystatus_old = pMystatus;
			string name(g_Control1Name);
			pMystatus = new CClientStatusRoom(room,name);
		}
		else
		{
			lastCommandResult = "创建房间失败！";
		}
	}
	if (menuID == 2)
	{
		memset(g_readString,0,30);
	
		printf("请输入房间ID\n");
		//key_getInt(roomID,5);
		getchar();
		scanf("%d",&roomID);
		CRoom room;
		if(jionRoom(room,roomID) == RTN_SUCCESS)
		{
			pMystatus_old = pMystatus;
			string name(g_Control1Name);
			pMystatus = new CClientStatusRoom(room,name);
		}
		else
		{
			lastCommandResult = "加入房间失败！";
		}
	}
}

void CClientStatusHouse::showOnlineList()
{
	printf("当前在线用户(%d位)：\n",m_UserList.size());
	for(map<string,CUser>::iterator it = m_UserList.begin(); it !=  m_UserList.end(); ++it)
	{
		printf("$ %s $ ",it->first.c_str());
	}
	printf("\n");
}


void CClientStatusHouse::showRoomList()
{
	printf("当前房间：\n");
	for(map<int,CRoom>::iterator it = m_roomList.begin(); it !=  m_roomList.end(); ++it)
	{
		string roomstat ="";
		if (it->second.getRoomStatus() == EROOM_STATUS_READY)
		{
			roomstat = "准备中";
		}
		else if (it->second.getRoomStatus() == EROOM_STATUS_GAMING)
		{
			roomstat = "游戏中";
		}
		else
		{
			roomstat = "已结束";
		}
		printf("$房间ID = %d ， 房间名称 = %s  创建者 = %s 房间人数 %d/%d 房间状态 %s $ \n "
			,it->second.getRoomID(),it->second.getRoomName().c_str(),it->second.gethostUserName().c_str()
			,it->second.getnowPlayer(),it->second.getmaxPlayer(),roomstat.c_str());
	}
	printf("\n");
}


int CClientStatusHouse::getOnlneList()
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskOnlineList(g_Control1Name,buf);

	//发送报文
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(ASK_ONLINE_USERLIST_ASW,buf,len) == RTN_SUCCESS)
	{
		m_UserList.clear();
		if (getProtocol()->ptclCheckAskOnlineListAsw( m_UserList,buf,len) == RTN_SUCCESS)
		return RTN_SUCCESS;
	}
	printf("getOnlneList error\n");
	return RTN_ERROR;
}

int CClientStatusHouse::getRoomList()
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskRoomList(g_Control1Name,buf);

	//发送报文
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(ROOMLIST_ASW,buf,len) == RTN_SUCCESS)
	{
		m_roomList.clear();
		if (getProtocol()->ptclCheckAskRoomListAsw( m_roomList,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("getOnlneList error\n");
	return RTN_ERROR;
}

int CClientStatusHouse::createRoom(CRoom& room, string roomName)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildCreateRoom(g_Control1Name,roomName,buf);

	//发送报文
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(CREATE_ROOM_ASW,buf,len) == RTN_SUCCESS)
	{
		if (getProtocol()->ptclCheckCreateRoomAsw(room,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("getOnlneList error\n");
	return RTN_ERROR;
}

int CClientStatusHouse::jionRoom(CRoom& room, int roomID)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildJionRoom(g_Control1Name,roomID,buf);

	//发送报文
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
		

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(CREATE_ROOM_ASW,buf,len) == RTN_SUCCESS)
	{
		if (getProtocol()->ptclCheckCreateRoomAsw(room,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("加入房间失败\n");
	return RTN_ERROR;
}