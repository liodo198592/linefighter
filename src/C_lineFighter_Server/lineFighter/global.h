#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>


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
#include "ace/Process_Mutex.h"
#include "ace/Process.h"
#include "ace/Message_Queue.h"

#include "logmonitor.h"

#include "stdio.h"

using namespace std;

struct SDate;

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


#define RTN_FINISH 1
#define RTN_SUCCESS 0
#define RTN_ERROR   -1
#define RTN_NOT_MATCH -2
#define RTN_TIME_OUT -3
#define RTN_FULL -4
#define RTN_EMPTY -5
#define RTN_RELOAD -6
#define RTN_CRITICAL -7
#define RTN_NOT_SEND  -8
#define RTN_PARAM_ERR  -9

enum EDERECTION
{
	EDERECTION_LEFT = 1,
	EDERECTION_RIGHT,
};


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

int LFclear(void);


void initPlace();

void DateToTime(SDate sDate,time_t &iTime);
void TimeToDate(SDate &sDate,time_t iTime);

struct SPlace
{
	int x;
	int y;
};

struct SDate
{
	unsigned char year;//正常年份减2000
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	SDate()
	{
		year = 0;
		month = 1;
		day = 1;
		hour = 0;
		min = 0;
		sec = 0;
	}

	SDate operator =(SDate sDate)
	{	
		year = sDate.year;
		month = sDate.month;
		day = sDate.day;
		hour = sDate.hour;
		min = sDate.min;
		sec = sDate.sec;
		return *this;
	}

	SDate nextMonth()
	{	
		month += 1;
		if (month == 13)
		{
			month = 1;
			year+= 1;
		}
		return *this;
	}

	SDate lastMonth()
	{
		month -= 1;
		if (month == 0)
		{
			month = 12;
			year -= 1;
		}
		return *this;
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
};

void OSTimeDly(int time);

int OSgetNowSeconds();

vector<string> stringSplit( string s,string pattern);


#endif