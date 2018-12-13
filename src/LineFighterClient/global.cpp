#include "global.h"
#include "channel.h"
ACE_RW_Thread_Mutex g_MutexTcpClient;
extern CTCPClient* g_pTcpClient;
ACE_UINT8 CheckSum(const ACE_UINT8 cBuffer[], ACE_INT32 iLen)
{
	ACE_UINT8 iResult = 0;
	for (ACE_INT32 i=0; i<iLen; i++)	
		iResult += cBuffer[i];
	return iResult;
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



//日期到秒数
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


//秒数到日期
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
int g_sendMessage(const unsigned char buf[], int len)
{
	g_MutexTcpClient.acquire();

	ACE_INT32 iResult = g_pTcpClient->send(buf,len);

	g_MutexTcpClient.release();
	return iResult;

}
int g_recvMessage(unsigned char buf[], size_t len)
{
	g_MutexTcpClient.acquire();

	ACE_Time_Value timeOut(0,100);
	ACE_INT32 iResult = g_pTcpClient->receive(buf,len,&timeOut);

	g_MutexTcpClient.release();

	return iResult;
}