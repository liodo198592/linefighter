#include "ConsoleShow.h"
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>

//��ǰ��5����ʾ
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
	printf("\n=======================�������ָ��=====================:\n");
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
	printf("\n=======================��ҵ�%d��ָ��=====================:\n",index);
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
		printf("=====================�������� ��%d��:=========================\n",it->first + 1);	
		showPlayer(vShowPlayer[it->first]);
		showComandSetp(it->first,vActions,vplayers);
		for(vector<string>::iterator itV = it->second.begin(); itV != it->second.end(); ++itV)
		{
			printf("%s , ",(*itV).c_str());
		}
		printf("\n====����1,�س�����һ��=====\n");
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
		printf("=====================��������:=========================\n");	
		for(map<int,vector<string> >::iterator it = vdiscreption.begin(); it != vdiscreption.end(); ++it)
		{
			printf("\n==��%d��==",it->first + 1);
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
			printf("\n==��%d��==",it->first + 1);
			for(vector<string>::iterator itV = it->second.begin(); itV != it->second.end(); ++itV)
			{
				printf("////////////////////////////////////////////////////////////////\n");
				printf("///////////////////��Ϸ���/////////////////////////////////////\n");
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
	printf("%s ���ջ�ʤ�� ��NB  �غ��� %d \n",player.getName().c_str(),index);
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
	printf("linefigher ��%d�غ�",index);
}

void waitAkey()
{
	int temp[20] = {0};
	printf("===================�����������================\n");
	//getch();
}

int getActionFromConsole(vector<CAction>& subVec, CPlayer player)
{
	int ActionId = 0,iDerection = 0 , X = 0 , Y = 0;
	if (player.getLifeStat() == ELIFESTAT_ALIFE)
	{
		printf("\n====ϵͳ��Ϣ==������");
		printf("%d:ȭ ",EACTION_Hand);
		printf("%d:�� ",EACTION_Air);
		printf("%d:ˤ ",EACTION_Fall);
		printf("%d:�ƶ� ",EACTION_Move);
		printf("%d:���� ",EACTION_ChangeLine);
		printf("%d:�ر� ",EACTION_Avoid);
		printf("%d:��ɱ 0:�ط� =======\n",EACTION_Kill);
	}
	else
	{
		printf("\n====ϵͳ��Ϣ====");
		printf("�������� =ֻ�ܹ�ս 0:�ط�======\n");
	}
	

	for(int i = 0 ; i< 3; ++i)
	{
		printf("��ѡ���%d������: ",i+1);
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
			printf("�����뷽�� 1:��  2����:");
			scanf("%d",&iDerection);
		}
		else if(ActionId == (int)EACTION_Move)
		{
			player.getPosition(X,Y);
			printf("�������ƶ�����λ�� Y����(1,2,3):");
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