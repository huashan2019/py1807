/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_led.c  ---   led灯控制
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.01
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
/********************************************************************************
**  Function	: LED_Ctl
**  Author		: 
**  Created on	: 20180723
**  Description	:
**  Return		: 
********************************************************************************/
void LED_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_LED_CTL, GPIO_PinOutput);
}
void LED_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		TurnOn_LED;
	}
	else
	{
		TurnOff_LED;
	}
}
/********************************************************************************
**  Function	: TASK_LED_pro
**  Author		: huwenjun
**  Created on	: 20180723
**  Description	:100ms执行一次
**  Return		: 
********************************************************************************/
void TASK_LED_pro(void)
{
	static SCH_U16 Timer=0;
	Timer++;
	if(Get_ArmUpdata_Flag)
	{
		if(Timer % T1S_100 == T_NOW)
		{
			LED_Ctl(ON);
		}
		else if(Timer % T1S_100 == T500MS_100)
		{
			LED_Ctl(OFF);
		}
	}
	else if(Get_SysPower_Flag)
	{
		LED_Ctl(ON);
	}
	else
	{
		LED_Ctl(OFF);
	}
}






