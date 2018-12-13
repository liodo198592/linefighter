#include "ConsoleShow.h"
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>

//在前面5行显示
void showPlayer(vector<CPlayer>& vplayers )
{
	map<int, string> m_player;
	int X = 0;
	int Y = 0;
	int HP = 0;
	char temp[1024] = {0};

	LcmSetCursor(3,0);
	printf("(1|1)------------------------(1|2)---------------------------(1|3)\n");
	LcmSetCursor(6,0);
	printf("(2|1)------------------------(2|2)---------------------------(2|3)\n");

	for(vector<CPlayer>::iterator it = vplayers.begin(); it != vplayers.end(); ++it)
	{
		if((*it).checkLifeStat() == ELIFESTAT_ALIFE)
		{
			(*it).getPosition(X,Y);
			HP = (*it).getHp();
			int index = X*10+Y;
			sprintf(temp,"%s(%d,%d),HP=%d",(*it).getName().c_str(),X,Y,HP);
			m_player[index] += (string)temp + "\n";
		}
		
	}

	for(map<int, string> :: iterator itM = m_player.begin(); itM != m_player.end(); ++itM)
	{
		LcmSetCursor((itM->first / 10 -1 ) * 3 + 2, (itM->first % 10 -1 ) * 20);
		printf("%s",itM->second.c_str());
	}
}

void showComand(map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers)
{
	if (vActions.empty())
	{
		return;
	}
	LcmSetCursor(7,0);
	printf("\n=======================玩家上轮指令=====================:\n");
	string strCommand = "";
	char temp[20] = {};
	for(int iPlayerIndex = 0 ; iPlayerIndex < MAX_PLAYER; ++iPlayerIndex)
	{
		if(vplayers[iPlayerIndex].checkLifeStat() == ELIFESTAT_ALIFE)
		{
			strCommand = "";
			strCommand += vplayers[iPlayerIndex].getName() + "===>";
			string strName = vplayers[iPlayerIndex].getName();
			for(int iActionIndex = 0; iActionIndex < MAX_ACTNUM; ++iActionIndex)
			{
				sprintf(temp,"%d",iActionIndex+1);
				strCommand += 	(string)temp + ":" + vActions[strName][iActionIndex].getDescription() + " ";
			}
			printf("%s:\n",strCommand.c_str());
		}
	}
}

void showComandSetp(int index,map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers)
{
	if (vActions.empty())
	{
		return;
	}
	LcmSetCursor(7,0);
	printf("\n=======================玩家第%d步指令=====================:\n",index);
	string strCommand = "";
	char temp[20] = {};
	for(int iPlayerIndex = 0 ; iPlayerIndex < MAX_PLAYER; ++iPlayerIndex)
	{
		if(vplayers[iPlayerIndex].checkLifeStat() == ELIFESTAT_ALIFE)
		{
			strCommand = "";
			strCommand += vplayers[iPlayerIndex].getName() + "===>";
			string strName = vplayers[iPlayerIndex].getName();
			sprintf(temp,"%d",index+1);	
			strCommand += 	(string)temp + ":" + vActions[strName][index].getDescription() + " ";
			printf("%s:\n",strCommand.c_str());
		}
	}
}

void showDetailByStep(map<int,vector<string> >& vdiscreption , map<int,vector<CPlayer> >& vShowPlayer,map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers)
{
	if (vdiscreption.empty())
	{
		return;
	}
	for(map<int,vector<string> >::iterator it = vdiscreption.begin(); it != vdiscreption.end(); ++it)
	{
		int d = 0;
		system("cls");
		printf("=====================过程演算 第%d步:=========================\n",it->first + 1);	
		showPlayer(vShowPlayer[it->first]);
		showComandSetp(it->first,vActions,vplayers);
		for(vector<string>::iterator itV = it->second.begin(); itV != it->second.end(); ++itV)
		{
			printf("%s , ",(*itV).c_str());
		}
		printf("\n====输入1,回车看下一步=====\n");
		getchar();
		scanf("%d",&d);
	}
}

void showDetail(map<int,vector<string> >& vdiscreption , map<int,vector<CPlayer> >& vShowPlayer,map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers)
{
	if (vdiscreption.empty())
	{
		return;
	}
	if (vdiscreption.size() > 1)
	{
		printf("=====================过程演算:=========================\n");	
		for(map<int,vector<string> >::iterator it = vdiscreption.begin(); it != vdiscreption.end(); ++it)
		{
			printf("\n==第%d步==",it->first + 1);
			for(vector<string>::iterator itV = it->second.begin(); itV != it->second.end(); ++itV)
			{
				printf("%s , ",(*itV).c_str());
			}
		}
	}
	else
	{
		for(map<int,vector<string> >::iterator it = vdiscreption.begin(); it != vdiscreption.end(); ++it)
		{
			printf("\n==第%d步==",it->first + 1);
			for(vector<string>::iterator itV = it->second.begin(); itV != it->second.end(); ++itV)
			{
				printf("////////////////////////////////////////////////////////////////\n");
				printf("///////////////////游戏结果/////////////////////////////////////\n");
				printf("/////%s////\n",(*itV).c_str());
				printf("/////////////////////////////////////////////////////////////////\n");
			    printf("/////////////////////////////////////////////////////////////////\n");
			}
		}
	}
	
}

void showGameOver(CPlayer player,int index)
{
	system("cls");
	printf("%s 最终获胜！ 真NB  回合数 %d \n",player.getName().c_str(),index);
}
void LcmSetCursor(int byRow, int byCol)
{
#ifdef MAKE_ANDRIOD
#else
	/*HANDLE hOut;
	int iRow = byRow + 1;
	COORD pos = {byCol,iRow };
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
	*/
#endif // MAKE_ANDRIOD
}


void showRoundIndex(int index)
{
	LcmSetCursor(0,0);
	printf("linefigher 第%d回合",index);
}

void waitAkey()
{
	int temp[20] = {0};
	printf("===================按任意键继续================\n");
	//getch();
}

int getActionFromConsole(vector<CAction>& subVec, CPlayer player)
{
	int ActionId = 0,iDerection = 0 , X = 0 , Y = 0;
	if (player.getLifeStat() == ELIFESTAT_ALIFE)
	{
		printf("\n====系统信息==请输入");
		printf("%d:拳 ",EACTION_Hand);
		printf("%d:气 ",EACTION_Air);
		printf("%d:摔 ",EACTION_Fall);
		printf("%d:移动 ",EACTION_Move);
		printf("%d:换线 ",EACTION_ChangeLine);
		printf("%d:回避 ",EACTION_Avoid);
		printf("%d:必杀 0:回放 =======\n",EACTION_Kill);
	}
	else
	{
		printf("\n====系统信息====");
		printf("您已死亡 =只能观战 0:回放======\n");
	}
	

	for(int i = 0 ; i< 3; ++i)
	{
		printf("请选择第%d步动作: ",i+1);
		scanf("%d",&ActionId);
		if (ActionId == 0)
		{
			return -4;
		}
		if (player.getLifeStat() != ELIFESTAT_ALIFE)
		{
			return RTN_ERROR;
		}
		if(ActionId == (int)EACTION_Air || ActionId == (int)EACTION_Kill)
		{
			printf("请输入方向 1:左  2：右:");
			scanf("%d",&iDerection);
		}
		else if(ActionId == (int)EACTION_Move)
		{
			player.getPosition(X,Y);
			printf("请输入移动到的位置 Y坐标(1,2,3):");
			scanf("%d",&Y);

		}	

		CAction p((EACTION)ActionId);
		if((EACTION)ActionId == EACTION_Move)
		{
			p.m_destX = X;
			p.m_destY = Y;
		}
		else if((EACTION)ActionId == EACTION_Air || (EACTION)ActionId == EACTION_Kill)
		{
			p.m_derection = iDerection;
		}
		subVec.push_back(p);
		printf("  ");
	}
	return RTN_SUCCESS;
}