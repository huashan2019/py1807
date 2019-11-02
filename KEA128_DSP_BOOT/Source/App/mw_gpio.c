/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_gpio.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.08.08
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"



GPIO_Type *GPIO_Arry[]={GPIOA,GPIOB,GPIOC};
SCH_BOOL GPIO_ReadInputPin(GPIO_PinType GPIO_Pin)
{
	return (GPIO_Arry[GPIO_Pin/32]->PDIR & (SCH_U32)(1<<(GPIO_Pin%32))) ? HIGH_LEVEL : LOW_LEVEL;
}
/********************************************************************************
**  Function    : ACC_Detect
**  Author      : wenjunHu
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T AccDetect;
void ACC_Init(void)
{///===ACC
	GPIO_PinInit(GPIO_ACC_DECT, GPIO_PinInput);
	sch_memset(&AccDetect, 0x00, sizeof(AccDetect));
}
void ACC_Detect(void)
{
	AccDetect.IO_Status = !ACC_DET_LEVEL;
	if(AccDetect.IO_Status != Get_ACC_Flag)
	{
		if(++AccDetect.IO_SamplingCounter >= T200MS_8)
		{
			AccDetect.IO_SamplingCounter = 0;
			if(AccDetect.IO_Status)
			{
				Set_ACC_Flag;
				Printf("ACC ON \n");
			}
			else
			{
				Clr_ACC_Flag;
				PostMessage(POWER_MODULE,EVT_POWER_OFF,POWEROFF_FROM_ACC);
				Printf("ACC OFF \n");
			}
		}	
	}
	else
	{
		AccDetect.IO_SamplingCounter = 0;
	}
}
/********************************************************************************
**  Function	: LED_Ctl
**  Author		: 
**  Created on	: 20180723
**  Description	:
**  Return		: 
********************************************************************************/
void LED_Init(void)
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


void SYS_Power_Ctl(SCH_BOOL OnOff)
{
	if(OnOff == ON)
	{
		TurnOn_SYS_POWER;
		Set_SysPower_Flag;
	}
	else
	{
		TurnOff_SYS_POWER;
		Clr_SysPower_Flag;
	}
}
/********************************************************************************
**  Function	: SYS_POWER
**  Author		: 
**  Created on	: 20161009
**  Description	:
**  Return		: 
********************************************************************************/
void SYS_PWR_Init(void)
{
	///GPIO_PinInit(GPIO_SYS_3V3_CTL, GPIO_PinOutput);
	///GPIO_PinInit(GPIO_SYS_12V_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_SYS_POWER_CTL, GPIO_PinOutput);
}
/********************************************************************************
**  Function	: USB_Ctl
**  Author		: 
**  Created on	: 20170427
**  Description	:
**  Return		: 
********************************************************************************/
void USB_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_USB_OC_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_USB_EN_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_USB1_OC_DECT, GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_USB1_EN_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_USB_POWER_CTL, GPIO_PinOutput);
}
/********************************************************************************
**  Function	: BT_Ctl
**  Author		: 
**  Created on	: 20171220
**  Description	:
**  Return		: 
********************************************************************************/
void BT_Init(void)
{///===
	GPIO_PinInit(GPIO_BT_RESET_CTL, GPIO_PinOutput);
}
/********************************************************************************
**  Function	: GPIOInit
**  Author		: wenjunHu
**  Created on	: 20160623
**  Description	: 
**  Return		: NULL
********************************************************************************/
void GPIOInit(void)
{
	BT_Init();
	ACC_Init();
	LED_Init();
	SYS_PWR_Init();
}



