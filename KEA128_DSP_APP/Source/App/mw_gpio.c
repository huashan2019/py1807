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
**  Function    : ARM_Ctl
**  Author      : 
**  Created on  : 20170427
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T NaviGateDetect;
void ARM_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_ARM_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_ARM_REV_CTL,   GPIO_PinOutput);
	///GPIO_PinInit(GPIO_SYS_POWER_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_ARM_WORK_DECT, GPIO_PinInput_InternalPullup);
}
/********************************************************************************
**  Function    : DSP_Test_Detect
**  Author      : wenjunHu
**  Created on  : 20180724
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T DSP_TEST_Detect;
void DSP_TEST_Init(void)
{///===
	GPIO_PinInit(GPIO_DSP_TEST_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&DSP_TEST_Detect, 0x00, sizeof(DSP_TEST_Detect));
}
SCH_BOOL DSP_OFF_FLAG = 0;
void DSP_Test_Detect(void)
{
	if(SysPower.nPowerState != POWER_NORMAL_RUN)
		return;
	DSP_TEST_Detect.IO_Status = DSP_TEST_DET_LVON;
	if(DSP_TEST_Detect.IO_Status != DSP_OFF_FLAG)
	{
		if(++DSP_TEST_Detect.IO_SamplingCounter >= T200MS_8)
		{
			DSP_TEST_Detect.IO_SamplingCounter = 0;
			if(DSP_TEST_Detect.IO_Status)
			{
				Set_DSP_OFF_Flag;
				DSP_OFF_FLAG = 1;
				Dsp_OFF();
			}
			else
			{
				Clr_DSP_OFF_Flag;
				DSP_OFF_FLAG = 0;
				Dsp_ON();
			}
		}	
	}
	else
	{
		DSP_TEST_Detect.IO_SamplingCounter = 0;
	}
}
/********************************************************************************
**  Function	: SYS_POWER
**  Author		: 
**  Created on	: 20161009
**  Description	:
**  Return		: 
********************************************************************************/
void PWR_IO_Init(void)
{
	///GPIO_PinInit(GPIO_SYS_3V3_CTL, GPIO_PinOutput);
	///GPIO_PinInit(GPIO_SYS_12V_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_SYS_POWER_CTL,  GPIO_PinOutput);
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
**  Function	: AMP
**  Author		: 
**  Created on	: 20160909
**  Description	:
**  Return		: 
********************************************************************************/
void AMP_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_AMP_CTL,   GPIO_PinOutput);
	GPIO_PinInit(GPIO_OPAMP_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_AMP_BEEP,  GPIO_PinOutput);
	GPIO_PinInit(GPIO_AMP_DECT,  GPIO_PinInput_InternalPullup);
}
/********************************************************************************
**  Function	: BT_IO_Ctl
**  Author		: 
**  Created on	: 20171220
**  Description	:
**  Return		: 
********************************************************************************/
void BT_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_BT_RESET_CTL, GPIO_PinOutput);
}
/********************************************************************************
**  Function	: USB_IO_Ctl
**  Author		: 
**  Created on	: 20180714
**  Description	:
**  Return		: 
********************************************************************************/
void USB_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_USB_OC_DECT,   GPIO_PinInput_InternalPullup);
	GPIO_PinInit(GPIO_USB_EN_CTL,    GPIO_PinOutput);
	GPIO_PinInit(GPIO_USB_POWER_CTL, GPIO_PinOutput);
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
	ARM_IO_Init();
	DSP_IO_Init();
	BT_IO_Init();
	DSP_TEST_Init();
	USB_IO_Init();
	Start_IO_Init();
	PWR_IO_Init();
	AMP_IO_Init();
	///MUTE_IO_Init();
	LED_IO_Init();
	AD1938_IO_Init();
	Eprom_IO_Init();
}



