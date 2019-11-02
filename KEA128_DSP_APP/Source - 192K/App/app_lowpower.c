/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : APP_lowpower.c  ---   低功耗
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/

#include "include.h"

void SysWakeUpInit_FromACC(void)
{
	SIM->SCGC |= SIM_SCGC_IRQ_MASK;
	SIM->PINSEL = (SIM->PINSEL & (~SIM_PINSEL_IRQPS_MASK)) | SIM_PINSEL_IRQPS(7);///3--PTI6
	IRQ->SC = IRQ_SC_IRQPDD_MASK/*|IRQ_SC_IRQEDG_MASK*/|IRQ_SC_IRQPE_MASK|IRQ_SC_IRQIE_MASK;
	NVIC_EnableIRQ(IRQ_IRQn);
}
void SysWakeUpExit_FromACC(void)
{
	SIM->SCGC &= ~SIM_SCGC_IRQ_MASK;
	SIM->PINSEL = (SIM->PINSEL & (~SIM_PINSEL_IRQPS_MASK)) | SIM_PINSEL_IRQPS(0);///0--PTA5  default 
	NVIC_DisableIRQ(IRQ_IRQn);
	ACC_Init();
}

void SysWakeUpInit_FromKBI(void)
{
	SCH_U32 i;
	KBI_ConfigType  sKBIConfig={0};
	for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
	{
		sKBIConfig.sPin[i].bEn	 = 0;
	}
	sKBIConfig.sBits.bRstKbsp   = 1;/*Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
	sKBIConfig.sBits.bKbspEn   = 1;/*The latched value in KBxSP register while interrupt flag occur to be read.*/
	sKBIConfig.sBits.bMode   = KBI_MODE_EDGE_ONLY;
	sKBIConfig.sBits.bIntEn  = 1;
	/*Rising edge/high level select; PTD5 channel enable(SW1 on TRK board) */
	sKBIConfig.sPin[KBI0_PTD0_SHIFT].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[KBI0_PTD0_SHIFT].bEn   = 1;
	KBI_Init(KBI0, &sKBIConfig);
	GPIO_PinInit(GPIO_PTD5, GPIO_PinInput_InternalPullup);
}
void SysWakeUpExit_FromKBI(void)
{
	KBI_DeInit(KBI0);
}

void SysWakeUpInit_FromRTC(void)
{
	
}
void SysWakeUpExit_FromRTC(void)
{
	
}
/*******************************************************************************\
* Function    : Reset I/O.          											*
* Input       :                                                                 *
* Output      :                                                                 *
* description :ACC off mode,reduce power consumption to lowest,reset all I/O	*
\*-----------------------------------------------------------------------------*/
void ResetIO(void)
{
	///GPIO_DeInit(GPIOA);
	///GPIO_DeInit(GPIOB);
	///GPIO_DeInit(GPIOC);
	GPIO_Init(GPIOA,0xFFFFFFFF,GPIO_PinInput);
	GPIO_Init(GPIOB,0xFFFFFFFF,GPIO_PinInput);
	GPIO_Init(GPIOC,0xFFFFFFFF,GPIO_PinInput);
}
void SysLowPowerInit(void)
{
	ResetIO();
	Bsp_SysTick_Close();
	///SysRtcExit();
	Bsp_WDOG_Close();
	SysUartExit(SCH_Uart0);
	SysUartExit(SCH_Uart1);
	SysUartExit(SCH_Uart2);
	SysSpiExit(SCH_Spi0);
	SysSpiExit(SCH_Spi1);
	///SysCanExit(SCH_Can0);
	///FTM0_PWM_Exit();
	SysAdcExit();
}
void SysLowPowerExit(void)
{
	Bsp_SysTick_Init();
	///Bsp_RTC_Init();
	Bsp_WDOG_Init();
	Bsp_UART_Init();
	Bsp_SPI_Init();
	///Bsp_CAN_Init();
	///Bsp_TIM_Init();
	Bsp_ADC_Init();
	Print_Init();
}
/**************************************************************************************
* FunctionName   : TASK_AccOff_Pro()
* Description    : 任务处理   ---     lowpower
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void TASK_AccOff_Pro(void)
{
	while(Get_SLEEP_Mode)
	{
		Printf("Enter lowerpowermode \n");
		FeedDog();
		SysLowPowerInit();
		Clr_AccWakeUP_Flag;
		SysWakeUpInit_FromACC();
	#if AUDIO_START == ENABLE
		if(!Get_ACC_Has)
			SysWakeUpInit_FromKBI();
	#endif
		SysWakeUpInit_FromRTC();
		while(!Get_AccWakeUP_Flag)
		{
			FeedDog();
			if((Get_ACC_Has&&ACC_DET_LVOFF)||(!Get_ACC_Has&&AUDIO_DET_LVOFF))
				PMC_SetMode(PMC,PmcModeStop3);
			if(AUDIO_DET_LVON||ACC_DET_LVON)
				Set_AccWakeUP_Flag;
		}
		FeedDog();
		SysWakeUpExit_FromRTC();
	#if AUDIO_START == ENABLE
		if(!Get_ACC_Has)
			SysWakeUpExit_FromKBI();
	#endif
		SysWakeUpExit_FromACC();
		SysLowPowerExit();
		Clr_SLEEP_Mode;
		Printf("Enter Normalmode \n");
		return;
	}
}




