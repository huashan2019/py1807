/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : system.c
**  Description : This file is the SYS.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

/********************************************************************************
**  Function	: SysDataInit
**  Author		: wenjunHu
**  Created on	: 20160623
**  Description	: 
**  Return		: NULL
********************************************************************************/
SYS_T Sys;
extern IO_DET_T AccDetect;

void SysDataInit(void)
{
	sch_memset(&Sys,0x00,sizeof(Sys));
	Set_SYS_MUTE;
	Set_VolErr_Flag;
	Set_ACC_Flag;
	
	AccDetect.IO_Status = ACC_DET_LVON;
	if(AccDetect.IO_Status)	Set_ACC_Has;
	SysPower.Power_First_Timer = 5000/100;
}






