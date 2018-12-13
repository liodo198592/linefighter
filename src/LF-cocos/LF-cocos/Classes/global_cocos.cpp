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

	// 和北京时间一致
	GetTimeFromSecond(tv.tv_sec + 8 * 3600, psCurTime);
#else
	//struct calendar sCal;
	time_t t1,t2;
	struct tm *tm;

	t1=time(NULL);
	tm=localtime(&t1);


	//实现获取当前时间
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

	//实现获取当前时间
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
        //如果是闰年，只有日期在三月之后才加一天
        if (mon > 1)
            leap = 1;
    }

    return (jdays [mon] + day + leap - 1);   //从1970年1月1日开始
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
    
    //找到相应的月份
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
** 函数名称: GetWeekDay
** 功能描述: 根据SDate结构体获得时间参数的星期数
*            1－6代表星期1－6  返回0- 星期天
** 输　  入: SDate *pSDate     －  时间结构体指针
** 输    出: 无
** 返 回 值: int     －  星期数，1－6代表星期1－6  返回0- 星期天
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

    if(sTime.day > 1)      //如果月结算日不是1号,算下月统计数据
    {
        u32Month += 1;
    }

    return u32Month;
}

/***********************************************************
** 函数名称: IsWeekChange
** 功能描述: 判断当前时间是否发生周变更
** 输　  入: SDate *psLastTime         － 最近一次周结算时间
**           SDate *psCurTime          － 当前时间
** 输  　出: SDate *psStartTime        － 第一个结算周开始时间
**           SDate *psEndTime          － 第一个结算周结束时间,也是下一周的开始时间
** 返 回 值: bool   true/false     跨周/没有跨周
************************************************************/
bool IsWeekChange(SDate& psLastTime, SDate& psCurTime, 
                  SDate& psStartTime, SDate& psEndTime)
{
    int byLastWeekDay;
    long dwCurDay, dwLastDay, dwStartDay, dwEndDay;

    dwCurDay  = GetDaySinceEpoch(psCurTime);
    dwLastDay = GetDaySinceEpoch(psLastTime);

    byLastWeekDay = GetWeekDay(psLastTime);

    //根据上次周统计时间计算周开始时间
    if(byLastWeekDay > 0)
    {
        dwStartDay = dwLastDay - byLastWeekDay + 1;   //星期一
    }
    else
    {
        dwStartDay = dwLastDay - 6;
    }
    dwEndDay   = dwStartDay + 7;                      //一周结束

    if(dwCurDay >= dwEndDay)
    {
        GetTimeFromDay(dwStartDay, psStartTime);      //计算一周的开始时标
        GetTimeFromDay(dwEndDay, psEndTime);          //计算一周的结束时标
        return true;
    }
    else
    {
        return false;
    }
}

/***********************************************************
** 函数名称: CalcMonthStartEndTime
** 功能描述: 根据月结算日、上次结算时间推算本次结算月开始时间和结束时间
** 输　  入: int u32LastTime            － 最近一次月结算时间
**           int   byMonthStatDay         － 月结算日
** 输  　出: int *pu32StartTime         － 第一个结算月开始时间
**           int *pu32EndTime           － 第一个结算月结束时间,下一结算月开始时间
** 返 回 值: 无
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

    if(sLastTime.day < byMonthStatDay)      //最近一次结算时间日小于月结算日
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
** 函数名称: IsMonthChange
** 功能描述: 根据月结算日、上次结算时间、当前时间信息判断月是否变更
** 输　  入: int u32LastTime            － 最近一次月结算时间
**           int u32CurTime             － 当前时间
**           int   byMonthStatDay         － 月结算日
** 输  　出: int *pu32StartTime         － 第一个结算月开始时间
**           int *pu32EndTime           － 第一个结算月结束时间,下一结算月开始时间
** 返 回 值: bool   true/false     跨月/没有跨月
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
** 函数名称: BinarySortInsert
** 功能描述: 用二分插入排序算法将一个新的数据插入到已排序的数组中
** 输　  入: int wInsertData       －  新的待排序的数据
**           int *pwSortArray      －  已排序的数组指针
**           int *piArrayLen      －  已排序的数组长度
**           int iMaxLen          －  需排序的数据最大个数
** 输    出: int *wSortArray       －  插入操作后排序好的数组指针
** 返 回 值: 无
** 说    明: 计算95概率大值时，可用插入排序算法排除最大的5%的数据即可。
************************************************************/
void BinarySortInsert(int wInsertData, int& pwSortArray, int& piArrayLen, int iMaxLen)
{
    int left, right, middle;
    int j;

    left = 0;
    right = piArrayLen-1;
    
    while( right >= left )
    {
        //　指向已排序好的中间位置
        middle = ( left + right ) / 2;
        if( wInsertData > (&pwSortArray)[middle] )
        {
            // 插入的元素在左区间
            right = middle-1;
        }
        else
        {
            //　插入的元素在右区间
            left = middle+1;
        }
    }

    //　后移排序码大于R[i]的记录
    for( j = piArrayLen-1; j >= left; j-- )
    {
        (&pwSortArray)[j+1] = (&pwSortArray)[j];
    }
    
    // 插入
    (&pwSortArray)[left] = wInsertData;

    if( piArrayLen < iMaxLen)   //只需排序iMaxLen长度
    {
        piArrayLen++;
    }
}

/******************************************************************************
* 函数名称: GetDaysofMonth 
* 函数功能: 获取某月的天数
* 输入参数: 
* 输出参数: 
* 返 回 值: 
******************************************************************************/
int GetDaysofMonth(SDate& pSDate)
{
    // 二月
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
    // 小月
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
* 函数名称: IsTimeValid 
* 函数功能: 判断时间是否合法
* 输入参数: SDate * pSDate 时间
* 输出参数: 
* 返 回 值: true/false
******************************************************************************/
bool IsTimeValid(SDate& pSDate)
{
    // 先判断年是否合法
    if (pSDate.year < 1970)
    {
        return false;
    }

    // 再判断月是否合法
    if ((pSDate.month < 1) || (pSDate.month > 12))
    {
        return false;
    }

    // 然后判断天是否合法
    if ((pSDate.day > GetDaysofMonth(pSDate)) || (pSDate.day < 1))
    {
        return false;
    }

    // 最后判时分秒
    if ((pSDate.hour >= 24) || (pSDate.min >= 60) || (pSDate.sec >= 60))
    {
        return false;
    }

    return true;
}

/******************************************************************************
* 函数名称: IsSameDay 
* 函数功能: 判断2个世纪秒时间是否是同一天
* 输入参数: int u32DateTime1, int u32DateTime2
* 输出参数: 无
* 返 回 值: true 是, false 否
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
* 函数名称: GetNormalMonthFromSecond 
* 函数功能: 根据世纪秒获取当前自然月份
* 输入参数: 
* 输出参数: 
* 返 回 值: 
******************************************************************************/
int GetNormalMonthFromSecond(long dwSeconds)
{
    return 1;
}

/******************************************************************************
* 函数名称: IsValidTime 
* 函数功能: 判断是否是合理的时间
* 输入参数: int u32Time 世纪秒时间
* 输出参数: 无
* 返 回 值: true 有效时间, false 无效时间
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


