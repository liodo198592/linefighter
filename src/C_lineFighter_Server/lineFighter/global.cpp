#include "global.h"
#include "model_comm.h"

//vector<CPlayer> vPlayerInfo; //玩家信息，当前状态，填命令

//CAction actResult[MAX_ACTNUM][MAX_PLAYER];//每个player的三步动作
//CPlayer playerResult[MAX_ACTNUM][MAX_PLAYER]; //每个player三步动作之后的状态，已经死亡的player状态不改变

//map<int,SPlace> g_mapCoordinate;//坐标：first为坐标编号，second为坐标位置

//vector<CPlayer> vPlayerInfo; //玩家信息，当前状态，填命令

//CAction actResult[MAX_ACTNUM][MAX_PLAYER];//每个player的三步动作
//CPlayer playerResult[MAX_ACTNUM][MAX_PLAYER]; //每个player三步动作之后的状态，已经死亡的player状态不改变

int SCFlag; //1 表示 服务器  2表示客户端

string selfName; //自身的名字

int statusFlag; //UI和engine 之间共享，用来决定是否进行下一步工作


int LFclear(void)
{
	return 0;
}

void DateToTime(SDate sDate,time_t &iTime)
{
	struct tm tm_struct;
	tm_struct.tm_year = sDate.year + 100;  //1900年开始的年份
	tm_struct.tm_mon = sDate.month - 1; // [0-11]
	tm_struct.tm_mday = sDate.day; // [1-31]
	tm_struct.tm_hour = sDate.hour; // [0-23]
	tm_struct.tm_min = sDate.min; // [0- 59]
	tm_struct.tm_sec = sDate.sec; // [0-59]
	iTime = ACE_OS::mktime(&tm_struct);
}

void TimeToDate(SDate &sDate,time_t iTime)
{
	ACE_Time_Value ta(iTime);
	ACE_Date_Time dateTime(ta);	
	sDate.year = (unsigned char)(dateTime.year() - 2000);
	sDate.month = (unsigned char)dateTime.month();
	sDate.day = (unsigned char)dateTime.day();
	sDate.hour = (unsigned char)dateTime.hour();
	sDate.min = (unsigned char)dateTime.minute();
	sDate.sec = (unsigned char)dateTime.second();
}

void OSTimeDly(int time)
{
	ACE_OS::sleep(1);
}

int OSgetNowSeconds()
{
	int iNow = ACE_OS::gettimeofday().sec();
	return iNow;
}

vector<string> stringSplit( string s,string pattern)
{ 
	vector<int>  pos; 
	int begin = 0;  
	while(true)
	{ 
		begin = (int)s.find(pattern,begin);  
		if(begin < 0 || begin >= (int)s.size()) break;  
		pos.push_back(begin); 
		begin += (int)pattern.size(); 
	} 
	vector<string> res; 
	if(pos.size() > 0)
	{ 
		if(pos[0] > 0)
		{ 
			res.push_back(s.substr(0,pos[0]));
		}   
		for(int i=1;i<(int)pos.size();++i)
		{ 
			if(0 <pos[i]-pos[i-1]-pattern.size())
			{   
				res.push_back( s.substr( pos[i-1]+pattern.size(),pos[i]-pos[i-1]-pattern.size()));   
			}   
		}  
		if(pos[pos.size()-1] + pattern.size()<= s.size()-1)
		{ 
			res.push_back( s.substr( pos[pos.size()-1]+pattern.size(),s.size()-pos[pos.size()-1]-pattern.size()));   
		}   
	}   
	return  res;   
}
