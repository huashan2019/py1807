/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : task.c
**  Description : This file is the SYS.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"


///===================================================
void Task_1ms_Pro(void)
{
	
}

void Task_2ms_Pro(void)
{
	
}

void Task_4ms_Pro(void)
{
	TASK_Voltage_Det();
}

void Task_8ms_Pro(void)
{
	TASK_Arm_Pro();
	TASK_Bt_Pro();
	TASK_Power_Pro();
	TASK_Eeprom_Pro();
	TASK_Dsp_Pro();
	TASK_Key_Pro();
	Start_Detect();
}

void Task_16ms_Pro(void)
{
	TASK_Amp_Pro();
}

void Task_100ms_Pro(void)
{
	if(Get_IAP_Mode)
		IAP_Pro();
	TASK_LED_pro();
	
	//Printf("APP Begin -- Software Version : %s \n", MCU_VERSION);
}
///=============================================================================================
void SCH_Task_Execute (void)
{
	/*if(F_1ms_Val)
	{
		F_1ms_Clr;
		Task_1ms_Pro();
	}
	if(F_2ms_Val)
	{
		F_2ms_Clr;
		Task_2ms_Pro();
	}*/
	if(F_4ms_Val)
	{
		F_4ms_Clr;
		Task_4ms_Pro();
	}
	if(F_8ms_Val)
	{
		F_8ms_Clr;
		Task_8ms_Pro();
	}
	if(F_16ms_Val)
	{
		F_16ms_Clr;
		Task_16ms_Pro();
	}
	if(F_100ms_Val)
	{
		F_100ms_Clr;
		if(SysPower.Power_First_Timer)	SysPower.Power_First_Timer--;
		Task_100ms_Pro();
	}
}

