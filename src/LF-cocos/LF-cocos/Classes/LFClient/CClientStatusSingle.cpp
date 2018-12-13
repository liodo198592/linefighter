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

CClientStatusSingle::CClientStatusSingle(CEngine * pEngine_x,CRoom room_x,string userName_x)
{
	this->pEngine = pEngine_x;
	this->RoundIndex = 0;
	this->LastRoundIndex = 0;
	this->isActed = 1;
	this->isGameOver = 0;
	this->userName = userName_x;
	this->room = room_x;
}

string CClientStatusSingle::getTitleInfo()
{
	char content[250] = {0};
	
	if (this->RoundIndex == ROUND_FINISH)
	{
		sprintf(content,"%s 本局游戏结束，总回合数：%d, 房间号 = %d",userName.c_str(), LastRoundIndex ,room.getRoomID());
	}
	else
	{
		sprintf(content,"%s 在游戏中 回合数：%d, 房间号 = %d",g_Control1Name, this->RoundIndex ,room.getRoomID());
	}
	string retStr(content);
	return retStr;
}


//此状态流程反转，受服务端驱动进行相应的操作
void CClientStatusSingle::showInfo()
{
	if (getPlayerInfo() == RTN_SUCCESS)
	{
		system("cls");

		if (this->RoundIndex == ROUND_FINISH)
		{
			this->isGameOver = 1;
			printf("%s 本局游戏结束，总回合数：%d  , 房间号 = %d\n",g_Control1Name, LastRoundIndex ,room.getRoomID());
			printf("////////////////////////////////////////////////////////////////\n");
		}
		else
		{
			printf("%s 在游戏中 回合数：%d  , 房间号 = %d\n",g_Control1Name, this->RoundIndex ,room.getRoomID());
			printf("////////////////////////////////////////////////////////////////\n");
			if (this->LastRoundIndex != this->RoundIndex)
			{
				//新的一轮开始了
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

 int CClientStatusSingle::getMe(CPlayer& p)
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

void CClientStatusSingle::showMenu()
{
	int menuID;
	if(isGameOver == 1)
	{
		printf("\n=====系统信息： 游戏已结束  请输入:1 返回大厅========\n");
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
		printf("\n====系统信息：请耐心等待其他玩家输入=========\n");
		OSTimeDly(5);
	}
}

int CClientStatusSingle::getPlayerInfo()
{
	vplayers.clear();
	vplayers = this->pEngine->vplayers;	unsigned char buf[2000];
	this->RoundIndex = this->pEngine->RoundIndex;
	return RTN_SUCCESS;
}

int CClientStatusSingle::getLastActionInfo()
{
	vShowActions.clear();
	vShowActions = this->pEngine->vShowActions;
	return RTN_SUCCESS;
}

int CClientStatusSingle::getFormatInfo()
{
	vformatdescription.clear();
	map<int,vector<string> > vTempFormat = pEngine->vFormatdiscreption;
	for (map<int,vector<string> >::iterator it = vTempFormat.begin(); it != vTempFormat.end(); ++it)
	{
		int vecSize = it->second.size();
		vector<CFormatUnit *> subVec;
		for (int j = 0 ; j < vecSize; ++j)
		{
			string strContent = (it->second)[j];
			vector<string> tempVec = stringSplit(strContent,":");
			if ( tempVec.size() > 0)
			{
				string title = tempVec[0];
				CFormatUnit * pUnit = CFormatUnitFactory::getInstance()->create(title);
				if (pUnit->decode(tempVec) == RTN_SUCCESS)
				{
					subVec.push_back(pUnit);
				}
			}
		}
		pair<int,vector<CFormatUnit *> > tp(it->first,subVec);
		vformatdescription.insert(tp);
	}
	return RTN_SUCCESS;
}

int CClientStatusSingle::getDetailInfo()
{	
	vdiscreption.clear();
	vdiscreption = pEngine->vdiscreption;
	return RTN_SUCCESS;
}

int CClientStatusSingle::getSendActions(vector<CAction>& subVec, int& RoundIndex)
{
	if (RoundIndex != pEngine->RoundIndex)
	{
		return RTN_ERROR;
	}
	pair<string ,vector<CAction> > tp(userName,subVec);
	pEngine->vActions.insert(tp);
	return RTN_SUCCESS;
}

int CClientStatusSingle::stepPlayerInfo()
{
	vdispPlayer.clear();
	vdispPlayer = pEngine->vdispPlayer;
	return RTN_SUCCESS;
}

int CClientStatusSingle::quitRoom(int roomID)
{
	return RTN_ERROR;
}
CPlayer* CClientStatusSingle::getPlayerByName(string strName)
{
	for(vector<CPlayer>::iterator it = vplayers.begin();it != vplayers.end();++it)
	{
		if(it->getName() == strName)
		{
			return &(*it);
		}
	}
}