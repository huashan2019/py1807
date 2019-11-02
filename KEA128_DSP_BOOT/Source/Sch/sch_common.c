/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : sch_common.c
**  Description : This file is the SCH.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "sch_config.h"



/*************************************************************************************
**  Function    : SysWaitXXX
**  Author		: wenjunHu
**  Created on	: 
**  Description	: µÈ´ıÊ±¼ä Delay
**  Return		: NULL
**************************************************************************************/
void SysWait1Us(void)
{
#if 1
	SCH_U8 index = 2;
	while(index--)
		SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;
#else
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
	SHC_NOP;SHC_NOP;SHC_NOP;
#endif
}
void SysWaitUs(SCH_U32 Time)
{
	while(Time--)
		SysWait1Us();
}
void SysWaitMs(SCH_U32 Time)
{
	while(Time--)
		SysWaitUs(1000);
}
/*************************************************************************************
**  Function    : GetMax GetMin
**  Author		: wenjunHu
**  Created on	: 20170608
**  Description	: ±È½Ï´óĞ¡º¯Êı
**  Return		: NULL
**************************************************************************************/
SCH_U32 GetMax(SCH_U32 m,SCH_U32 n)
{
	return (m>n)?m:n;
}
SCH_U32 GetMin(SCH_U32 m,SCH_U32 n)
{
	return (m>n)?n:m;
}
/*************************************************************************************
**  Function    : sch_memxxx
**  Author		: wenjunHu
**  Created on	: 20170608
**  Description	: ¿½±´º¯Êı
**  Return		: NULL
**************************************************************************************/
void sch_memset(void *s,const SCH_U8 c,SCH_U16 n)  
{
    SCH_U8 *su = (SCH_U8 *)s;
	while(n--)
	{
		*su++ = c;
	}
}  
void sch_memcpy(void *to,  void const *from, SCH_U16 n)  
{
	SCH_U8 *t = (SCH_U8 *)to;
	SCH_U8 *f = (SCH_U8 *)from;
	while(n--)
	{
		*t++ = *f++;
	}
}  
/*************************************************************************************
**  Function    : GetCheckData 
**  Author		: wenjunHu
**  Created on	: 20170608
**  Description	: »ñÈ¡Ğ£ÑéÂë
**  Return		: NULL
**************************************************************************************/
SCH_U8 GetCheckData_Add(SCH_U8 *data, SCH_U8 Len)
{
	SCH_U8 index=0;
	while(Len--)
		index += *data++;
	return index;
}
SCH_U8 GetCheckData_Xor(SCH_U8 *data, SCH_U8 Len)
{
	SCH_U8 index=0;
	while(Len--)
		index ^= *data++;
	return index;
}
/*************************************************************************************
**  Function    :  IsInXXX
**  Author		: wenjunHu
**  Created on	: 20170703
**  Description	: ÅĞ¶ÏÒ»¸öÕûÊıÊÇ·ñÔÚÄ³¸ö·¶Î§ÄÚ
**  Return		: NULL
**************************************************************************************/
SCH_BOOL IsInRange(SCH_U32 min,SCH_U32 data,SCH_U32 max)
{
	if(min > max)
		return FALSE;
	return (data >= min && data <= max) ? TRUE : FALSE;
}
SCH_BOOL IsInData(SCH_U32 data0,SCH_U32 data1,SCH_U32 step)
{
	SCH_U32 index;
	index = (data0 > data1) ? (data0 - data1) : (data1 - data0);
	return (index <= step) ? TRUE : FALSE;
}
/*************************************************************************************
**  Function    : GetAverage_UXX
**  Author		: wenjunHu
**  Created on	: 20170803
**  Description	: ÇóÒ»¸öÊı×éµÄÆ½¾ùÖµ
**  Return		: NULL
**************************************************************************************/
SCH_U16 GetAverage_U16(SCH_U16 *pTable, SCH_U8 Array_Cnt)
{
	SCH_U32 Data = 0x00000000;
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		Data += pTable[index];
	}
	return (SCH_U16)(Data/Array_Cnt);
}
SCH_U8 GetAverage_U8(SCH_U8 *pTable, SCH_U8 Array_Cnt)
{
	SCH_U16 Data = 0x0000;
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		Data += pTable[index];
	}
	return (SCH_U8)(Data/Array_Cnt);
}
/*************************************************************************************
**  Function    : IsInArray_UXX
**  Author		: wenjunHu
**  Created on	: 20170803
**  Description	: ÇóÒ»¸öÊıÔÚÊı×éµÄÎ»ÖÃ(ÏÂ±ê)
**  Return		: NULL
**************************************************************************************/
SCH_BOOL IsInArray_U8(SCH_U8 Data,SCH_U8 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num)
{
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		if(pTable[index] == Data)
		{
			*num = index;
			return TRUE;
		}
	}
	return FALSE;
}
SCH_BOOL IsInArray_U16(SCH_U16 Data,SCH_U16 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num)
{
	SCH_U8 index;
	for(index=0;index<Array_Cnt;index++)
	{
		if(pTable[index] == Data)
		{
			*num = index;
			return TRUE;
		}
	}
	return FALSE;
}













