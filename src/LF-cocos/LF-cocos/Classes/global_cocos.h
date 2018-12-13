#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>

#include "stdio.h"

using namespace std;

struct SDate;

//FRAME��С
#define FRAME_WIDTH 120
#define FRAME_HEIGHT 126
#define FRAME_SIZE_MAX 120,126
#define FRAME_SIZE_HALF 120,63
#define FRAME_SIZE_QUARTER 60,63

//�����С
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
	EACTION_Hand = 1,//ȭ��ֻ�ܹ���ͬλ�õ���
	EACTION_Air ,//�������Թ���λ�õ�X���Լ���ͬ����
	EACTION_Fall,//ˤ���Է�����رܣ����Բ���˫������
	EACTION_Move,//�ƶ������Ըı�λ�����������X��X��ΧΪ1.2.3
	EACTION_ChangeLine,//���Ըı�λ�����������Y��Y��ΧΪ1,2
	EACTION_Avoid,//�ر�//���Իرܳ���EACTION_Fall֮������й���
	EACTION_Kill,//��ɱ //�����������˺���������3��
	EACTION_Invalid,//��Ч //�����κζ���
};

int LFclear(void);


void initPlace();

void DateToTime(SDate sDate,time_t &iTime);
void TimeToDate(SDate &sDate,time_t iTime);

void OSTimeDly(float delay_time);
void OSgetSysTime(SDate& psCurTime);
time_t  OSgetNowSeconds(void);


struct SPlace
{
	int x;
	int y;
};

struct SDate
{
	unsigned char year;//������ݼ�2000
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

typedef struct 
{
    int tm_sec;     /* Seconds: 0-59 (K&R says 0-61?) */
    int tm_min;     /* Minutes: 0-59 */
    int tm_hour;    /* Hours since midnight: 0-23 */
    int tm_mday;    /* Day of the month: 1-31 */
    int tm_mon;     /* Months *since* january: 0-11 */
    int tm_year;    /* Years since 1900 */
    int tm_wday;    /* Days since Sunday (0-6) */
    int tm_yday;    /* Days since Jan. 1: 0-365 */
    int tm_isdst;   /* +1 Daylight Savings Time, 0 No DST,
                 * -1 don't know */
}STime;

long GetSecondSinceEpoch(SDate& SDate);
bool GetTimeFromSecond(long dwSeconds, SDate& time);
vector<string> stringSplit( string s,string pattern);

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 450

#endif