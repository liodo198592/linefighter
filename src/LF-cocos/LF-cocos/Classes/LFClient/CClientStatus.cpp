#include "CClientStatus.h"
#include "CProtocol_code.h"
#include "CProtocol.h"

vector<int> keyVec;
extern char g_readString[300];
extern int showKeyFlag;

CCmdStatus * pMystatus;
CCmdStatus * pMystatus_old;

int CCmdStatus::recvAReportFormServer(int needCode ,unsigned char * pbuf , int& len)
{
	int codeinbuf = 0;
	int iInterTime = OSgetNowSeconds();
	int iNow = 0;
	while (true)
	{
		if(getProtocol()->recvAReportFormServer(pbuf,len) == RTN_SUCCESS)
		{
			codeinbuf = getProtocol()->ptclGetFuncCode(pbuf,len);
			if (needCode == codeinbuf)
			{
				return RTN_SUCCESS;
			}
			else if (codeinbuf == CHAT)
			{
				getProtocol()->ptclCheckChatInfoAsw(qChatqueue,pbuf,len);
				continue;
			}
		}
		iNow = OSgetNowSeconds();
		if (iNow - iInterTime > 6)
		{
			return RTN_ERROR;
		}
	}
	return RTN_ERROR;
}

//菜单限0~10
int CCmdStatus::key_getMenuButten(int& MenuID , int timeout)
{
	int count = 0;
	keyVec.clear();
	printf("keyVec.clear();\n");
	showKeyFlag = 1;
	while(true)
	{
		OSTimeDly(1);
		count++;
		for (int i = 0; i < keyVec.size(); ++i)
		{
			if (keyVec[i] >= 48 && keyVec[i] < 57)
			{
				keyVec.clear();
				MenuID = keyVec[i];
				showKeyFlag = 0;
				return RTN_SUCCESS;
			}
		}
		
		if(count > timeout * 2)
		{
			showKeyFlag = 0;
			return RTN_TIME_OUT;
		}
	}
	showKeyFlag = 0;
	return RTN_ERROR;
}

//连续字符直到回车
int CCmdStatus::key_getString(string& str , int timeout)
{
	int count = 0;
	keyVec.clear();
	printf("keyVec.clear();\n");
	showKeyFlag = 1;
	while(true)
	{
		memset(g_readString,0,sizeof(g_readString));
		OSTimeDly(0.5);
		if (keyVec.size() > 299)
		{
			showKeyFlag = 0;
			return RTN_ERROR;
		}
		for (int i = 0; i < keyVec.size(); ++i)
		{
			g_readString[i] = keyVec[i];
			printf("$%c$",keyVec[i]);
			if (keyVec[i] == 10)//有回车
			{
				string tempStr(g_readString);
				str = tempStr;
				showKeyFlag = 0;
				return RTN_SUCCESS;
			}
		}
		count++;
		if(count > timeout * 2)
		{
			showKeyFlag = 0;
			return RTN_TIME_OUT;
		}
	}
	showKeyFlag = 0;
	return RTN_ERROR;
}

//连续字符直到回车
int CCmdStatus::key_getInt(int& intval , int timeout)
{
	int count = 0;
	int tempValue = 0;
	keyVec.clear();
	printf("keyVec.clear();\n");
	showKeyFlag = 1;
	while(true)
	{
		OSTimeDly(1);
		if (keyVec.size() > 299)
		{
			showKeyFlag = 0;
			return RTN_ERROR;
		}
		for (int i = 0; i < keyVec.size(); ++i)
		{
			if (keyVec[i] > 48 && keyVec[i] < 58)
			{
				tempValue = tempValue * 10 + keyVec[i];
			}
			if (keyVec[i] == 10)//有回车
			{
				intval = tempValue;
				showKeyFlag = 0;
				return RTN_SUCCESS;
			}
		}
		count++;
		if(count > timeout * 2)
		{
			showKeyFlag = 0;
			return RTN_TIME_OUT;
		}
	}
	showKeyFlag = 0;
	return RTN_ERROR;
}