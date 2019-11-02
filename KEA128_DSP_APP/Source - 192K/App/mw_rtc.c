/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_rtc.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

ClockStruct Clock;
/**************************************************************
**  Function    : 
**  Author		: wenjunHu
**  Created on	: 20160623
**  Description	: 
**  Return		: NULL
**************************************************************/
SCH_U8 GetMaxDay(SCH_U8 Month,SCH_U8 leap)
{
  	SCH_U8  Max;
  	const SCH_U8 MaxDay[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  	Max=MaxDay[Month-1];
  	if(Month==2&&leap==1)
  	{
		Max=29;
  	}
  	return(Max);
}
/************************************************************************
**  Function    : 
**  Author      : 
**  Created on  : 
**  Description : 
**  Return      : NULL
************************************************************************/
const SCH_U8 bMonthTable[12] = {1,4,4,7,2,5,7,3,6,1,4,6};
SCH_U8 GetClockDay(int wYear,SCH_U8 bMonth,SCH_U8 bDate)
{
	int ret;
  	ret = wYear +((wYear-1)/4)-((wYear-1)/100) +((wYear-1)/400);
  	ret += bMonthTable[bMonth-1] ;
  	ret += ((bMonth>2)&&_IsLeapYear(wYear))?1:0;
  	ret += bDate +5;
  	ret %= 7;	
  	if(0 == ret)
  	{
		ret = 7;
  	}
  	return((SCH_U8)ret);
}
/************************************************************************
**  Function    : 
**  Author      : 
**  Created on  : 
**  Description : 
**  Return      : NULL
************************************************************************/
void GetCurrentTime(void)
{
	SCH_U8 maxday,tempyear,tempmonth,tempday;
	tempday=Clock.date;
	tempyear=Clock.year;
	tempmonth=Clock.month;
	Clock.sec++;
	if(Clock.sec>=60)
	{
		Clock.sec=0;
		Clock.min++;
		if(Clock.min>=60)
		{	
			Clock.min=0;
			Clock.hour++;		
			if(Clock.hour>=24)
			{
				Clock.hour=0;
				tempday++;
				maxday=GetMaxDay(Clock.month,_IsLeapYear(2000+Clock.year));
				if(tempday>maxday)
				{	
					tempday=tempday-maxday;
					tempmonth++;
					if(tempmonth>12)
					{
						tempmonth=1;
						tempyear++;
						if(tempyear==100)
						{
							tempyear=0;
						}
					}		
				}
				Clock.date=tempday;
				Clock.year=tempyear;
				Clock.month=tempmonth;
				Clock.day=GetClockDay(2000+Clock.year,Clock.month,Clock.date);
			}
		}
	}	
}
/************************************************************************
**  Function    : GetRTCClock
**  Author      : wenjunHu
**  Created on  : 
**  Description : 100ms÷¥––“ª¥Œ
**  Return      : NULL
************************************************************************/
void GetRTCClock(void)
{
	static SCH_U8 gTm100msCount;
	gTm100msCount++;
	if(gTm100msCount==10)
	{
		gTm100msCount=0;
		GetCurrentTime();
	}	
}
/********************************************************************************
**  Function    : RTCInit
**  Author      : wenjunHu
**  Created on  : 20160623
**  Description : RTC
**  Return      : NULL
********************************************************************************/
void RTCInit(void)
{
	Clock.sec=0;
	Clock.min=0;
	Clock.hour=0;
	Clock.day=6;
	Clock.date=1;
	Clock.month=1;
	Clock.year=16;
}

///=============================================================================================================
void SysRtcExit(void)
{
	RTC_DeInit();
}
void SysRtcInit(void)
{
	RTC_ConfigType t_RTC_Config;                         /* Config struct */
	t_RTC_Config.u16ModuloValue = 9;
	t_RTC_Config.bInterruptEn   = RTC_INTERRUPT_ENABLE;  /* Enable interrupt */
	t_RTC_Config.bClockSource   = RTC_CLKSRC_1KHZ;       /* Clock source is 1khz */
	t_RTC_Config.bClockPresaler = RTC_CLK_PRESCALER_100; /* Prescaler is 100 */
	RTC_Init(&t_RTC_Config);                            /* Write config */
}

