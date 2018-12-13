#include "global.h"
#include "CAction.h"
#include "CPlayer.h"
#include "CEngine.h"
#include "statusStream/status_comm.h"
#include "CProtocol.h"
#include "logmonitor.h"
#include "channel.h"
#include <process.h>
#include <stdio.h>
#include <conio.h>

char g_Control1Name[30];
//int g_iTimeOut = 5;
int showKeyFlag = 0;

extern SRingBuf g_ringBuffer;
CCmdStatus * pMystatus;
CCmdStatus * pMystatus_old;

vector<int> keyVec;

int getKey()
{
	int ch = 0;
	while(true)
	{
		ch = getch();
		keyVec.push_back(ch);
		if (showKeyFlag == 1)
		{
			printf("%c keyVec.size = %d",ch,keyVec.size());

		}
	}
	return 0;
}

int main(int argc , char ** argv)
{
	 LFclear();//初始化clear
	 RBInit(&g_ringBuffer);
	 pMystatus_old = NULL;
	 pMystatus = new CClientStatusLogIn();
	
	 unsigned int pid;
	 int count=0 , i = 0;
	 // _beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))getKey,(void *)count,0,(unsigned int *)&pid);


	 while (true)
	 {
		 if(pMystatus_old != NULL)
		 {
			delete pMystatus_old;
			pMystatus_old = NULL;
		 }
		 system("cls");
		 pMystatus->showInfo();
		 pMystatus->showMenu();
		 ACE_OS::sleep(0.5);	
	 }

	 return 0;
}