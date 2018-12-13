#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <cstring>
#include <cstdlib>
//STL
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>
#include <set>


//ACE
#include "ace/Task.h"
#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Time_Value.h"
#include "ace/Message_Block.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Mutex.h"
#include "ace/Guard_T.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/DEV_Addr.h"
#include "ace/DEV_Connector.h"
#include "ace/DEV_IO.h"
#include "ace/TTY_IO.h"
#include "ace/Date_Time.h"
#include "ace/streams.h"
#include "ace/Service_Config.h"
#include "ace/Codecs.h"

#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "logmonitor.h"
//#include "configmanager.h"
using namespace std;


class CProtocol;
struct SDate;
class CFrame;


#define PORT_ADDR_1 "COM1"
#define PORT_ADDR_2 "COM2"
#define PORT_ADDR_3 "COM3"
#define PORT_ADDR_4 "COM4"
#define PORT_ADDR_5 "COM5"
#define PORT_ADDR_6 "COM6"
#define PORT_ADDR_7 "COM7"
#define PORT_ADDR_8 "COM8"
#define PORT_ADDR_9 "COM9"
#define PORT_ADDR_10 "\\\\.\\COM10"
#define PORT_ADDR_11 "\\\\.\\COM11"
#define PORT_ADDR_12 "\\\\.\\COM12"
#define PORT_ADDR_13 "\\\\.\\COM13"
#define PORT_ADDR_14 "\\\\.\\COM14"
#define PORT_ADDR_15 "\\\\.\\COM15"


#define QUEUE_SIZE  1024*1024*100
#define REPORT_LENGTH  1024

#define SEC_DAY 86400
#define SEC_HOUR 3600
#define SEC_5MIN 300


#define PROTOCOL_HEAD 10
#define CONTROL_LENGTH 3//控制码和长度码长度
#define PROTOCOL_TAIL 2//验证码和结尾长度

#define SET_PARA_FRAME_SEQUENCE 0x7F
#define DATA_FRAME_SEQUENCE     0x01
#define DATA_ERROR_TASK_RECALL  0x7E
#define READ_PARA               0x45

#define RTN_SUCCESS 0
#define RTN_ERROR -1
#define RTN_TIME_OUT -2

//FRAME大小
#define FRAME_WIDTH 120
#define FRAME_HEIGHT 126
#define FRAME_SIZE_MAX 120,126
#define FRAME_SIZE_HALF 120,63
#define FRAME_SIZE_QUARTER 60,63

//字体大小
#define FONT_SIZE_MAX 12
#define FONT_SIZE_MIN 10


#define MAX_HP 15
#define MAX_PLAYER 4
#define MAX_ACTNUM 3


enum EACTION
{
	EACTION_Hand = 1,//拳
	EACTION_Air ,//气
	EACTION_Fall,//摔
	EACTION_Move,//移动
	EACTION_ChangeLine,//换线
	EACTION_Avoid,//回避
	EACTION_Kill,//必杀
	EACTION_Invalid,//无效
};


int g_sendMessage(const unsigned char buf[], int len);
int g_recvMessage(unsigned char buf[], size_t len);

//日期到秒数
void DateToTime(SDate sDate,time_t &iTime);

//秒数到日期
void TimeToDate(SDate &sDate,time_t iTime);


vector<string> stringSplit( string s,string pattern);
struct SDate
{
	unsigned char year;//正常年份减2000
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	SDate(int iYear,int iMonth,int iDay,int iHour,int iMin,int iSec = 0)
	{
		year = iYear;
		month = iMonth;
		day = iDay;
		hour = iHour;
		min = iMin;
		sec = iSec;
	}

	SDate()
	{
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		min = 0;
		sec = 0;
	}

	SDate& operator =(const SDate& sDate)
	{	
		year = sDate.year;
		month = sDate.month;
		day = sDate.day;
		hour = sDate.hour;
		min = sDate.min;
		sec = sDate.sec;
		return *this;
	}

	void lastMonth()
	{
		month -= 1;
		if (month == 0)
		{
			year -= 1;
			month = 12;
		}
	}

	void nextMonth()
	{
		month += 1;
		if (month == 13)
		{
			year += 1;
			month = 1;
		}
	}

	void nextDay() //后一天
	{
		time_t iTimeThis; 
		DateToTime(*this, iTimeThis);
		TimeToDate(*this, iTimeThis + SEC_DAY);
	}

	void lastDay() //前一天
	{
		time_t iTimeThis; 
		DateToTime(*this, iTimeThis);
		TimeToDate(*this, iTimeThis - SEC_DAY);
	}

	void reset()
	{
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		min = 0;
		sec = 0;
	}

	bool operator < (SDate rDate)
	{
		time_t iTimeThis , iTimeR; 
		DateToTime(*this ,iTimeThis);
		DateToTime(rDate ,iTimeR);
		return (iTimeThis<iTimeR);
	}

	bool operator >= (SDate rDate)
	{
		time_t iTimeThis , iTimeR; 
		DateToTime(*this ,iTimeThis);
		DateToTime(rDate ,iTimeR);
		return (iTimeThis>=iTimeR);
	}

	string toString()
	{
		char strValue[32] = {};
		sprintf(strValue,"%d-%d-%d %d:%d:%d",year,month,day,hour,min,sec);
		return (string)strValue;
	}
	int fromString(string strTime)
	{
		vector<string> vecStr = stringSplit(strTime," ");
		if(vecStr.size() != 2)
		{
			return -1;
		}
		vector<string> vecDate = stringSplit(vecStr[0],"-");
		if(vecDate.size() != 3)
		{
			return -1;
		}
		vector<string> vecTime = stringSplit(vecStr[1],":");
		if(vecTime.size() != 3)
		{
			return -1;
		}

		this->year = atoi(vecDate[0].c_str()) - 2000;
		this->month = atoi(vecDate[1].c_str());
		this->day = atoi(vecDate[2].c_str());

		this->hour = atoi(vecTime[0].c_str());
		this->min = atoi(vecTime[1].c_str());
		this->sec = atoi(vecTime[2].c_str());
	}
	bool isValid()
	{
		if(QDate::isValid(year+2000,month,day) && QTime::isValid(hour,min,sec))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

ACE_UINT8 CheckSum(const ACE_UINT8 cBuffer[], ACE_INT32 iLen);
ACE_INT8 ByteToBcd(const ACE_UINT8 cByte);
ACE_INT8 BcdToByte(const ACE_UINT8 cBcd);



#endif