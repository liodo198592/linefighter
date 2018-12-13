#include "CProtocol.h"
#ifdef LF_COCOS 
#include "channel_cocos.h"
#else
#include "channel.h"
#endif
#include "CFormatUnit.h"

CFormatUnitFactory * CFormatUnitFactory::myFactory = NULL;

//////////////////////////////////////////////////////////////////////////CAttackUnit
int CAttackUnit::decode(vector<string> str)
{
	if (str.size() < 5)
	{
		return RTN_ERROR;
	}
	attackerName = str[1];
	sufferName = str[2];
	if (strcmp(str[3].c_str(),"ATTACKED") == 0)
	{
		isAttacked = ATTACKED;
	}
	else
	{
		isAttacked = MISSED;
	}
	damege = atoi(str[4].c_str());
	return RTN_SUCCESS;
}

string CAttackUnit::encode()
{
	string temp  = "attack:"+attackerName+":"+sufferName+":";
	char dmgBuf[20];
	sprintf(dmgBuf,"%d",damege);
	string dmgStr(dmgBuf);
	if (isAttacked == ATTACKED)
	{
		temp += "ATTACKED:";
	}
	else
	{
		temp += "MISSED:";
	}
	temp += dmgStr;
	return temp;
}

//////////////////////////////////////////////////////////////////////////CMoveUnit
int CMoveUnit::decode(vector<string> str)
{
	if (str.size() < 4)
	{
		return RTN_ERROR;
	}
	
	username = str[1];
	sscanf(str[2].c_str(),"%d,%d",&srcx,&srcy);
	sscanf(str[3].c_str(),"%d,%d",&destx,&desty);
	return RTN_SUCCESS;
}

string CMoveUnit::encode()
{
	string temp  = "move:"+username+":";
	char dmgBuf[50];
	sprintf(dmgBuf,"%d:%d:%d:%d",srcx,srcy,destx,desty);
	string dmgStr(dmgBuf);
	temp += dmgStr;
	return temp;
}

//////////////////////////////////////////////////////////////////////////CMissUnit
int CMissUnit::decode(vector<string> str)
{
	if (str.size() < 2)
	{
		return RTN_ERROR;
	}
	username = str[1];
	return RTN_SUCCESS;
}

string CMissUnit::encode()
{
	string temp  = "miss:"+username;
	return temp;
}