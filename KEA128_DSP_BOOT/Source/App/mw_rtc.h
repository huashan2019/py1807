///=========================
#ifndef __MW_RTC_H
#define __MW_RTC_H

typedef struct
{
	SCH_U8 sec;
	SCH_U8 min;
	SCH_U8 hour;
	SCH_U8 day;
	SCH_U8 date;
	SCH_U8 month;
	SCH_U8 year;
} ClockStruct;
extern ClockStruct Clock;

#define _IsLeapYear(_Year_)	(((0 == (_Year_ % 4)) && (0 != (_Year_ % 100))) || (0 == (_Year_ % 400)))



extern void GetRTCClock(void);
extern void RTCInit(void);
extern void GetCurrentTime(void);
extern void SysRtcExit(void);
extern void SysRtcInit(void);
#endif
