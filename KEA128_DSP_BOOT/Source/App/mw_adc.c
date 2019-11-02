/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_adc.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

SCH_BOOL ADC_GetData(SCH_U8 u8Channel,SCH_U16 *ADC_Data)
{
	*ADC_Data = (SCH_U16)ADC_PollRead(ADC,u8Channel);
	return TRUE;
}

void SysAdcExit(void)
{
	ADC_DeInit(ADC);
}

void SysAdcInit(void)
{
	ADC_ConfigType  t_ADC_Config={0};                       
	t_ADC_Config.u8ClockDiv      = ADC_ADIV_DIVIDE_4;      
	t_ADC_Config.u8ClockSource   = CLOCK_SOURCE_BUS_CLOCK; 
	t_ADC_Config.u8Mode          = ADC_MODE_12BIT;
	///t_ADC_Config.sSetting.bIntEn = 1;
    ADC_Init(ADC, &t_ADC_Config);
}



