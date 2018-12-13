#include "status_comm.h"
#ifdef MAKE_ANDRIOD
#include "ConsoleShow.h"
#else
#include "../ConsoleShow.h"
#endif // MAKE_ANDRIOD


#ifdef LF_COCOS 
#include "channel_cocos.h"
#else
#include "channel.h"
#endif

extern char g_Control1Name[30];
extern int g_iTimeOut;
extern CCmdStatus * pMystatus;
extern CCmdStatus * pMystatus_old;
extern char g_readString[300];

CClientStatusGame::CClientStatusGame(CRoom room_x,string userName_x)
{
	this->RoundIndex = 0;
	this->LastRoundIndex = 0;
	this->isActed = 1;
	this->isGameOver = 0;
	this->userName = userName_x;
	this->room = room_x;
}

string CClientStatusGame::getTitleInfo()
{
	char content[250] = {0};
	
	if (this->RoundIndex == ROUND_FINISH)
	{
		sprintf(content,"%s ������Ϸ�������ܻغ�����%d, ����� = %d",userName.c_str(), LastRoundIndex ,room.getRoomID());
	}
	else
	{
		sprintf(content,"%s ����Ϸ�� �غ�����%d, ����� = %d",g_Control1Name, this->RoundIndex ,room.getRoomID());
	}
	string retStr(content);
	return retStr;
}


//��״̬���̷�ת���ܷ��������������Ӧ�Ĳ���
void CClientStatusGame::showInfo()
{
	if (getPlayerInfo() == RTN_SUCCESS)
	{
		system("cls");

		if (this->RoundIndex == ROUND_FINISH)
		{
			this->isGameOver = 1;
			printf("%s ������Ϸ�������ܻغ�����%d  , ����� = %d\n",g_Control1Name, LastRoundIndex ,room.getRoomID());
			printf("////////////////////////////////////////////////////////////////\n");
		}
		else
		{
			printf("%s ����Ϸ�� �غ�����%d  , ����� = %d\n",g_Control1Name, this->RoundIndex ,room.getRoomID());
			printf("////////////////////////////////////////////////////////////////\n");
			if (this->LastRoundIndex != this->RoundIndex)
			{
				//�µ�һ�ֿ�ʼ��
				this->LastRoundIndex = this->RoundIndex;
				this->isActed = 0;
			}
		}
		showPlayer(vplayers);
	}
	if (getLastActionInfo() == RTN_SUCCESS)
	{
		showComand(vShowActions,vplayers);
	}
	if (getDetailInfo() == RTN_SUCCESS)
	{
		if (stepPlayerInfo() == RTN_SUCCESS)
		{
			showDetail(vdiscreption,vdispPlayer,vShowActions,vplayers);
		}
	}
	return;
}

 int CClientStatusGame::getMe(CPlayer& p)
{
	for (int i = 0 ; i < vplayers.size(); ++i)
	{
		if (strcmp(g_Control1Name,vplayers[i].getName().c_str()) == 0)
		{
			p = vplayers[i];
			return RTN_SUCCESS;
		}
	}
	return RTN_ERROR;
}

void CClientStatusGame::showMenu()
{
	int menuID;
	if(isGameOver == 1)
	{
		printf("\n=====ϵͳ��Ϣ�� ��Ϸ�ѽ���  ������:1 ���ش���========\n");
		getchar();
		scanf("%d",&menuID);
		if (menuID == 1)
		{
			if (quitRoom(room.getRoomID()) == RTN_SUCCESS)
			{
				pMystatus_old = pMystatus;
				pMystatus = new CClientStatusHouse();
				return;
			}
		}
		else
		{
			return;
		}
	}

	if (isActed == 0)
	{
		CPlayer p;
		int ret = getMe(p);
		vector<CAction> subVec;
		if (ret == RTN_SUCCESS)
		{
			int returnValue = getActionFromConsole(subVec,p);
			if (returnValue == -4)
			{
				showDetailByStep(vdiscreption,vdispPlayer,vShowActions,vplayers);
			}
			else if (returnValue == RTN_SUCCESS)
			{
				if (getSendActions(subVec,RoundIndex) == RTN_SUCCESS)
				{
					isActed = 1;
					return;
				}
			}
		}
	}
	else
	{
		printf("\n====ϵͳ��Ϣ�������ĵȴ������������=========\n");
		OSTimeDly(5);
	}
}

int CClientStatusGame::getPlayerInfo()
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskPlayersInGame(g_Control1Name,room.getRoomID(),buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(ASK_PLAYER_INFO_INGAME_ASW,buf,len) == RTN_SUCCESS)
	{
		vplayers.clear();
		int rd = 0;
		if (getProtocol()->ptclCheckAskPlayersInGameAsw( vplayers ,rd,buf,len) == RTN_SUCCESS)
		{
			this->RoundIndex = rd;
			return RTN_SUCCESS;
		}
		
	}
	printf("getPlayerInfo error\n");
	return RTN_ERROR;
}

int CClientStatusGame::getLastActionInfo()
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskActinsInGame(g_Control1Name,room.getRoomID(),buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
	}

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(Get_OTHER_ACTION_INGAME_ASW,buf,len) == RTN_SUCCESS)
	{
		vShowActions.clear();
		if (getProtocol()->ptclCheckAskActinsInGameAsw( vShowActions ,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("getLastActionInfo error\n");
	return RTN_ERROR;
}

int CClientStatusGame::getFormatInfo()
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskFormatInGame(g_Control1Name,room.getRoomID(),buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{

	}

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(Get_FORMATRESULT_INGAME_ASW,buf,len) == RTN_SUCCESS)
	{
		vformatdescription.clear();
		if (getProtocol()->ptclCheckAskFormatInGameAsw( vformatdescription ,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("getDetailInfo error\n");
	return RTN_ERROR;
}

int CClientStatusGame::getDetailInfo()
{	
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskDetailInGame(g_Control1Name,room.getRoomID(),buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	
	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(Get_RESULT_INGAME_ASW,buf,len) == RTN_SUCCESS)
	{
		vdiscreption.clear();
		if (getProtocol()->ptclCheckAskDetailInGameAsw( vdiscreption ,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("getDetailInfo error\n");
	return RTN_ERROR;
}

int CClientStatusGame::getSendActions(vector<CAction>& subVec, int& RoundIndex)
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskSendAction(g_Control1Name,room.getRoomID(),subVec,RoundIndex,buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(SEND_ACTION_INGAME_ASW,buf,len) == RTN_SUCCESS)
	{
		if (getProtocol()->ptclCheckAskSendActionAsw(buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("getDetailInfo error\n");
	return RTN_ERROR;
}

int CClientStatusGame::stepPlayerInfo()
{
	unsigned char buf[2000];
	int ret  = 0;
	int len = getProtocol()->ptclBuildAskStepPlayerInGame(g_Control1Name,room.getRoomID(),buf);

	//���ͱ���
	ret = getConn()->CtcpSend(buf,len);
	if (ret <= RTN_SUCCESS)
	{
		
	}
	

	memset(buf,0,sizeof(buf));

	if(recvAReportFormServer(ASKSTEP_PLAYERINFO_ASW,buf,len) == RTN_SUCCESS)
	{
		if (getProtocol()->ptclCheckAskStepPlayerInGameAsw( vdispPlayer,buf,len) == RTN_SUCCESS)
			return RTN_SUCCESS;
	}
	printf("stepPlayerInfo error\n");
	return RTN_ERROR;
}

int CClientStatusGame::quitRoom(int roomID)
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
CPlayer* CClientStatusGame::getPlayerByName(string strName)
{
	for(vector<CPlayer>::iterator it = vplayers.begin();it != vplayers.end();++it)
	{
		if(it->getName() == strName)
		{
			return &(*it);
		}
	}
}