#ifdef LF_COCOS 
#include "global_cocos.h"
#include <time.h>
#ifdef MAKE_ANDRIOD
#else
#include <windows.h>
#endif
#else
#include "global.h"
#endif
#include "model_comm.h"

#define  SECONDS_PER_DAY 86400
#define isleap(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

int LFclear(void)
{
	return 0;
}

void DateToTime(SDate sDate,time_t &iTime)
{
	iTime = GetSecondSinceEpoch(sDate);
}

void TimeToDate(SDate &sDate,time_t iTime)
{
	GetTimeFromSecond(iTime,sDate);
}

void OSTimeDly(float delay_time)
{
#ifdef MAKE_ANDRIOD
	sleep(delay_time);
#else
	Sleep(delay_time * 1000);
#endif // MAKE_ANDRIOD
}

void OSgetSysTime(SDate& psCurTime)
{
#ifdef MAKE_ANDRIOD
	struct timeval  tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	tv.tv_sec = tv.tv_sec;

	// �ͱ���ʱ��һ��
	GetTimeFromSecond(tv.tv_sec + 8 * 3600, psCurTime);
#else
	//struct calendar sCal;
	time_t t1,t2;
	struct tm *tm;

	t1=time(NULL);
	tm=localtime(&t1);


	//ʵ�ֻ�ȡ��ǰʱ��
	//rtc_gettime(&sCal);

	//GetTimeFromSecond(tv_sec, psCurTime);
	psCurTime.year    = tm->tm_year + 1900;
	psCurTime.month  = tm->tm_mon + 1;
	psCurTime.day    = tm->tm_mday;
	psCurTime.hour   = tm->tm_hour;
	psCurTime.min = tm->tm_min;
	psCurTime.sec = tm->tm_sec;
#endif // MAKE_ANDRIOD

	
}

time_t  OSgetNowSeconds(void)
{
	time_t tv_sec = 0;
	SDate sCurTime;

	//ʵ�ֻ�ȡ��ǰʱ��
	OSgetSysTime(sCurTime);

	tv_sec = GetSecondSinceEpoch(sCurTime);

	return tv_sec;
}

/************************************************************************
*
*  daysSinceEpoch - calculate number days since ANSI C epoch                 
* 
*  The (year + 1)/4 term accounts for leap years, the     
*  first of which was 1972 & should be added starting '73 
* 
* RETURNS:
* NOMANUAL
************************************************************************/

int __daysSinceEpoch_db( int year, int yday )
{
    if(year>=0) /* 1970 + */
        return ( (365 * year) + (year + 1) / 4  + yday );
    else        /* 1969 - */
        return ( (365 * year) + (year - 2) / 4  + yday );
} 

/************************************************************************
*
* julday - calculate Julian Day given year, month, day            
*              Inputs      : year (years since 1900), month (0 - 11), 
*                      day (1 - 31)  
*              Comment     : Returns Julian day in range 1:366.  
*                 Unix wants 0:365 
* RETURNS: Julian day                                            
* NOMANUAL
************************************************************************/
#define TM_YEAR_BASE 0

int __julday_db(int yr, int mon, int day)
{
    static int jdays[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int leap = 0;

    if (isleap (yr + TM_YEAR_BASE))
    {
        //��������ֻ꣬������������֮��ż�һ��
        if (mon > 1)
            leap = 1;
    }

    return (jdays [mon] + day + leap - 1);   //��1970��1��1�տ�ʼ
}

 /************************************************************************
 *
 * GetSecondSinceEpoch--Get Seconds since 1970
 *        Comment     : Returns Seconds of the Time since Epoch.  
 * RETURNS: Seconds Num.
 ************************************************************************/

long GetSecondSinceEpoch(SDate& SDate)
{
    int julday;
    int day;

    if (SDate.year<100)
    {
        SDate.year += 2000 ;
    }

    julday = __julday_db(SDate.year, SDate.month-1, SDate.day);
    
    day = __daysSinceEpoch_db(SDate.year-1970,julday);

    return( ( (day*24 + SDate.hour)*60 + SDate.min )*60 +SDate.sec);
}

 
long GetMinuteSinceEpoch(SDate& SDate)
{
    if (SDate.year<100)
    {
        SDate.year += 2000 ;
    }
    return GetSecondSinceEpoch(SDate)/60;
}
 
 
long GetDaySinceEpoch(SDate& SDate)
{
    if (SDate.year<100)
    {
        SDate.year += 2000 ;
    }
    return GetMinuteSinceEpoch(SDate)/1440;
}


/************************************************************************
*
* __getTime - convert calendar time into broken-down time
*
* internal routine.
*
* RETURNS: OK
* NOMANUAL
************************************************************************/
#define SECSPERMIN      60  /* Seconds per minute */
#define MINSPERHOUR     60  /* minutes per hour */
#define HOURSPERDAY     24  /* hours per day */
#define DAYSPERWEEK     7   /* days per week */
#define MONSPERYEAR     12  /* months per year */
#define SECSPERHOUR     (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY      ((long) SECSPERHOUR * HOURSPERDAY)

#define DAYSPERYEAR     365 /* days per non-leap year */

#define CENTURY         100 /* years per century */

#define TM_THURSDAY 4   /* Thursday is the fourth day of the week */
#define TM_SUNDAY   0   /* Sunday is the zeroth day of the week */
#define TM_MONDAY   1   /* Monday is the first day of the week */

#define EPOCH_WDAY      TM_THURSDAY
#define EPOCH_YEAR      1970



int getTime_tm (long timer, STime& tmp)
{
    long days ;
    long timeOfDay ;
    long year ;
    long mon ;
    
    days = timer / SECSPERDAY ;
    timeOfDay = timer % SECSPERDAY ;
    
    if(timeOfDay < 0)
    {
        timeOfDay += SECSPERDAY ;
        days -= 1 ;
    }
    
    year = days / DAYSPERYEAR ;
    while(__daysSinceEpoch_db(year,0)>days)
    year--;
    
    tmp.tm_wday=(days+EPOCH_WDAY)%DAYSPERWEEK ;
    
    if(tmp.tm_wday<0)
    tmp.tm_wday+=DAYSPERWEEK ;
    
    days-=__daysSinceEpoch_db(year,0);
    year+=EPOCH_YEAR ;
    
    //�ҵ���Ӧ���·�
    for(mon=0;(days>=__julday_db(year-TM_YEAR_BASE,mon+1,1))&&(mon<11);mon++)
    ;

    tmp.tm_year=year-TM_YEAR_BASE ;

    tmp.tm_year = mon ;
    tmp.tm_mday = (days-__julday_db(tmp.tm_year, mon, 0))  ;
    tmp.tm_yday = __julday_db(tmp.tm_year,mon,tmp.tm_mday)-1 ;
    tmp.tm_hour = timeOfDay / SECSPERHOUR ;
    timeOfDay %= SECSPERHOUR ;
    tmp.tm_min = timeOfDay / SECSPERMIN ;
    tmp.tm_sec = timeOfDay % SECSPERMIN ;
    
    return(true);
}





/************************************************************************
*
* GetTimeFromSecond - convert calendar time into broken-down time
*                     with struct SDate
* internal routine.
*
* RETURNS: OK
* NOMANUAL
************************************************************************/
bool GetTimeFromSecond(long dwSeconds, SDate& time)
{
    STime  tmp ;
    
    getTime_tm (dwSeconds, tmp);

    time.year    = tmp.tm_year  ;
    time.month  = tmp.tm_mon+1 ;
    time.day    = tmp.tm_mday  ;
    time.hour   = tmp.tm_hour  ;
    time.min = tmp.tm_min   ;
    time.sec = tmp.tm_sec   ;

    if((0 == time.day) && (1 == time.month))
    {
        time.year--;
        time.month = 12;
        time.day = 31;
    }

    return true ;
}

bool GetTimeFromMinute(long dwMinute, SDate& time)
{
    return GetTimeFromSecond(dwMinute*60, time);
}

bool GetTimeFromDay(long dwDay, SDate& time)
{
    return GetTimeFromMinute(dwDay*1440, time);
}

int GetTimeFromMonth(int dwMonth)
{
    SDate sTime;
    
    sTime.year   = EPOCH_YEAR + dwMonth/12;
    sTime.month = dwMonth%12;
    sTime.day   = 1;
    sTime.hour  = 0;
    sTime.min = 0;
    sTime.sec = 0;

    return (GetSecondSinceEpoch(sTime));
}


/************************************************************************
** ��������: GetWeekDay
** ��������: ����SDate�ṹ����ʱ�������������
*            1��6��������1��6  ����0- ������
** �䡡  ��: SDate *pSDate     ��  ʱ��ṹ��ָ��
** ��    ��: ��
** �� �� ֵ: int     ��  ��������1��6��������1��6  ����0- ������
************************************************************************/
int GetWeekDay(SDate& pSDate)
{
    return ((GetDaySinceEpoch(pSDate)+3)%7);
}

void GetLastMonth(SDate& sTime)
{
    sTime.month -= 1;
    if(sTime.month == 0)
    {
        sTime.month = 12;
        sTime.year -= 1;
    }
}

void GetNextMonth(SDate& sTime)
{
    sTime.month += 1;
    if(sTime.month == 13)
    {
        sTime.month = 1;
        sTime.year += 1;
    }
}

int GetMonthFromSecond(long dwSeconds)
{
    int u32Month;
    SDate sTime;

    GetTimeFromSecond(dwSeconds, sTime);

    u32Month = (sTime.day - EPOCH_YEAR) * 12 + sTime.month;

    if(sTime.day > 1)      //����½����ղ���1��,������ͳ������
    {
        u32Month += 1;
    }

    return u32Month;
}

/***********************************************************
** ��������: IsWeekChange
** ��������: �жϵ�ǰʱ���Ƿ����ܱ��
** �䡡  ��: SDate *psLastTime         �� ���һ���ܽ���ʱ��
**           SDate *psCurTime          �� ��ǰʱ��
** ��  ����: SDate *psStartTime        �� ��һ�������ܿ�ʼʱ��
**           SDate *psEndTime          �� ��һ�������ܽ���ʱ��,Ҳ����һ�ܵĿ�ʼʱ��
** �� �� ֵ: bool   true/false     ����/û�п���
************************************************************/
bool IsWeekChange(SDate& psLastTime, SDate& psCurTime, 
                  SDate& psStartTime, SDate& psEndTime)
{
    int byLastWeekDay;
    long dwCurDay, dwLastDay, dwStartDay, dwEndDay;

    dwCurDay  = GetDaySinceEpoch(psCurTime);
    dwLastDay = GetDaySinceEpoch(psLastTime);

    byLastWeekDay = GetWeekDay(psLastTime);

    //�����ϴ���ͳ��ʱ������ܿ�ʼʱ��
    if(byLastWeekDay > 0)
    {
        dwStartDay = dwLastDay - byLastWeekDay + 1;   //����һ
    }
    else
    {
        dwStartDay = dwLastDay - 6;
    }
    dwEndDay   = dwStartDay + 7;                      //һ�ܽ���

    if(dwCurDay >= dwEndDay)
    {
        GetTimeFromDay(dwStartDay, psStartTime);      //����һ�ܵĿ�ʼʱ��
        GetTimeFromDay(dwEndDay, psEndTime);          //����һ�ܵĽ���ʱ��
        return true;
    }
    else
    {
        return false;
    }
}

/***********************************************************
** ��������: CalcMonthStartEndTime
** ��������: �����½����ա��ϴν���ʱ�����㱾�ν����¿�ʼʱ��ͽ���ʱ��
** �䡡  ��: int u32LastTime            �� ���һ���½���ʱ��
**           int   byMonthStatDay         �� �½�����
** ��  ����: int *pu32StartTime         �� ��һ�������¿�ʼʱ��
**           int *pu32EndTime           �� ��һ�������½���ʱ��,��һ�����¿�ʼʱ��
** �� �� ֵ: ��
************************************************************/
void CalcMonthStartEndTime(int u32LastTime, int byMonthStatDay, 
                           int& pu32StartTime, int& pu32EndTime)
{
    SDate sLastTime, sStartTime, sEndTime;

    GetTimeFromSecond(u32LastTime, sLastTime);
    memcpy(&sEndTime, &sLastTime, sizeof(SDate));
    sEndTime.hour   = 0;
    sEndTime.min = 0;
    sEndTime.sec = 0;

    if(sLastTime.day < byMonthStatDay)      //���һ�ν���ʱ����С���½�����
    {
        sEndTime.day = byMonthStatDay;
        memcpy(&sStartTime, &sEndTime, sizeof(SDate));
        GetLastMonth(sStartTime);
    }
    else
    {
        sEndTime.day     = byMonthStatDay;
        memcpy(&sStartTime, &sEndTime, sizeof(SDate));
        GetNextMonth(sEndTime);
    }

    pu32StartTime = GetSecondSinceEpoch(sStartTime);
    pu32EndTime   = GetSecondSinceEpoch(sEndTime);
}

/***********************************************************
** ��������: IsMonthChange
** ��������: �����½����ա��ϴν���ʱ�䡢��ǰʱ����Ϣ�ж����Ƿ���
** �䡡  ��: int u32LastTime            �� ���һ���½���ʱ��
**           int u32CurTime             �� ��ǰʱ��
**           int   byMonthStatDay         �� �½�����
** ��  ����: int *pu32StartTime         �� ��һ�������¿�ʼʱ��
**           int *pu32EndTime           �� ��һ�������½���ʱ��,��һ�����¿�ʼʱ��
** �� �� ֵ: bool   true/false     ����/û�п���
************************************************************/
bool IsMonthChange(int u32LastTime, int u32CurTime, int byMonthStatDay, 
                   int& pu32StartTime, int& pu32EndTime)
{
    CalcMonthStartEndTime(u32LastTime, byMonthStatDay, 
                          pu32StartTime, pu32EndTime);

    if(u32CurTime >= pu32EndTime)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/***********************************************************
** ��������: BinarySortInsert
** ��������: �ö��ֲ��������㷨��һ���µ����ݲ��뵽�������������
** �䡡  ��: int wInsertData       ��  �µĴ����������
**           int *pwSortArray      ��  �����������ָ��
**           int *piArrayLen      ��  ����������鳤��
**           int iMaxLen          ��  �����������������
** ��    ��: int *wSortArray       ��  �������������õ�����ָ��
** �� �� ֵ: ��
** ˵    ��: ����95���ʴ�ֵʱ�����ò��������㷨�ų�����5%�����ݼ��ɡ�
************************************************************/
void BinarySortInsert(int wInsertData, int& pwSortArray, int& piArrayLen, int iMaxLen)
{
    int left, right, middle;
    int j;

    left = 0;
    right = piArrayLen-1;
    
    while( right >= left )
    {
        //��ָ��������õ��м�λ��
        middle = ( left + right ) / 2;
        if( wInsertData > (&pwSortArray)[middle] )
        {
            // �����Ԫ����������
            right = middle-1;
        }
        else
        {
            //�������Ԫ����������
            left = middle+1;
        }
    }

    //���������������R[i]�ļ�¼
    for( j = piArrayLen-1; j >= left; j-- )
    {
        (&pwSortArray)[j+1] = (&pwSortArray)[j];
    }
    
    // ����
    (&pwSortArray)[left] = wInsertData;

    if( piArrayLen < iMaxLen)   //ֻ������iMaxLen����
    {
        piArrayLen++;
    }
}

/******************************************************************************
* ��������: GetDaysofMonth 
* ��������: ��ȡĳ�µ�����
* �������: 
* �������: 
* �� �� ֵ: 
******************************************************************************/
int GetDaysofMonth(SDate& pSDate)
{
    // ����
    if (pSDate.month == 2)
    {
        if (isleap(pSDate.year)) 
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    // С��
    else if ((pSDate.month == 4)
          || (pSDate.month == 6)
          || (pSDate.month == 9)
          || (pSDate.month == 11)) 
    {
        return 30;
    }
    else
    {
        return 31;
    } 
}

/******************************************************************************
* ��������: IsTimeValid 
* ��������: �ж�ʱ���Ƿ�Ϸ�
* �������: SDate * pSDate ʱ��
* �������: 
* �� �� ֵ: true/false
******************************************************************************/
bool IsTimeValid(SDate& pSDate)
{
    // ���ж����Ƿ�Ϸ�
    if (pSDate.year < 1970)
    {
        return false;
    }

    // ���ж����Ƿ�Ϸ�
    if ((pSDate.month < 1) || (pSDate.month > 12))
    {
        return false;
    }

    // Ȼ���ж����Ƿ�Ϸ�
    if ((pSDate.day > GetDaysofMonth(pSDate)) || (pSDate.day < 1))
    {
        return false;
    }

    // �����ʱ����
    if ((pSDate.hour >= 24) || (pSDate.min >= 60) || (pSDate.sec >= 60))
    {
        return false;
    }

    return true;
}

/******************************************************************************
* ��������: IsSameDay 
* ��������: �ж�2��������ʱ���Ƿ���ͬһ��
* �������: int u32DateTime1, int u32DateTime2
* �������: ��
* �� �� ֵ: true ��, false ��
******************************************************************************/
bool IsSameDay(int u32DateTime1, int u32DateTime2)
{
    if ((u32DateTime1 - u32DateTime1 % SECONDS_PER_DAY) == (u32DateTime2 - u32DateTime2 % SECONDS_PER_DAY))
    {
        return true;
    }

    return false;
}

/******************************************************************************
* ��������: GetNormalMonthFromSecond 
* ��������: �����������ȡ��ǰ��Ȼ�·�
* �������: 
* �������: 
* �� �� ֵ: 
******************************************************************************/
int GetNormalMonthFromSecond(long dwSeconds)
{
    return 1;
}

/******************************************************************************
* ��������: IsValidTime 
* ��������: �ж��Ƿ��Ǻ����ʱ��
* �������: int u32Time ������ʱ��
* �������: ��
* �� �� ֵ: true ��Чʱ��, false ��Чʱ��
******************************************************************************/
bool IsValidTime(int u32Time)
{
    return true;
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


