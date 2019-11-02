/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : bsp.c  ---   
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.08.10
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

////==============================================================================================Relocation ===================
#define RELOCATION_ADDRESS               0x4000 ///16K
void Bsp_Relocation_Int(void)
{
	write_vtor(RELOCATION_ADDRESS);
}
////=================================================================================================================
void Bsp_Clock_Init(void)
{
	sysinit();///40M
	/*ICS_ConfigType sICSConfig = {0};
	sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE;
	sICSConfig.oscConfig.bRange = 1;          
	sICSConfig.oscConfig.bGain = 1;
	sICSConfig.oscConfig.bStopEnable = 1;
    sICSConfig.oscConfig.bEnable = 1;      
    sICSConfig.u32ClkFreq = EXT_CLK_FREQ_KHZ;
	ICS_Init(&sICSConfig);*/
}

////=================================================================================================================
void Bsp_SysTick_Init(void)
{
	SysTick->VAL = 0x0;
    SysTick->LOAD = 160000-1;///40000*4=4ms///0x00FFFFFF;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	NVIC_EnableIRQ (SysTick_IRQn);
}
void Bsp_SysTick_Close(void)
{
	systick_disable();
	///SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk; 
}
////=================================================================================================================
void Bsp_GPIO_Init(void)
{	
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIOInit();///
}
////=================================================================================================================
/********************************************************************************
**  Function	: WDOG
**  Author		: WenjunHu
**  Created on	: 20170406
**  Description	: 
**  Return		: NULL
********************************************************************************/
#ifdef WDOG_EN
void Bsp_WDOG_Init(void)
{
	WDOG_ConfigType t_WDOG_Config;
	t_WDOG_Config.sBits.bWaitEnable   = 1;
	t_WDOG_Config.sBits.bStopEnable   = 0;  
	t_WDOG_Config.sBits.bDbgEnable    = 0; 
	t_WDOG_Config.sBits.bUpdateEnable = 1; 
	t_WDOG_Config.sBits.bDisable      = 0;                     /* enable WDOG */
	t_WDOG_Config.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;/*  */
	t_WDOG_Config.u16TimeOut          = 4000;                  /* 1s */
	t_WDOG_Config.u16WinTime          = 0;                     /*  */
	WDOG_Init(&t_WDOG_Config);                                 /*  */
	WDOG_Enable();
}
void FeedDog(void)
{
	WDOG_Feed();
}
void Bsp_WDOG_Close(void)
{
	///WDOG_DeInit();
	///WDOG_Disable();
	///WDOG_DisableWDOGEnableUpdate();
}
#endif
////=================================================================================================================
void Bsp_RTC_Init(void)
{
	///SysRtcInit();
}
////=================================================================================================================
void Bsp_PIT_Init(void)
{
    PIT_ConfigType  t_PIT_Config;       
	t_PIT_Config.u32LoadValue = 200000; 
    t_PIT_Config.bFreeze      = 0;     
    t_PIT_Config.bModuleDis   = 0;     /* Enable PIT module */
    t_PIT_Config.bInterruptEn = 1;     
    t_PIT_Config.bChainMode   = 1;       
    t_PIT_Config.bTimerEn     = 1;     
    PIT_Init (PIT_CHANNEL0, &t_PIT_Config);
}
void Bsp_TIM_Init(void)
{
	///Bsp_PIT_Init();
	///FTM0_PWM_Init();////CCFL±≥π‚PWM
}
////=================================================================================================================
#define UART0_BAUDRATE       115200///
#define UART1_BAUDRATE       115200///
#define UART2_BAUDRATE       115200///
void Bsp_UART_Init(void)
{
	SysUartInit(SCH_Uart0,UART0_BAUDRATE);
	SysUartInit(SCH_Uart1,UART1_BAUDRATE);
	SysUartInit(SCH_Uart2,UART2_BAUDRATE);
}
////=================================================================================================================
#define SPI0_BAUDRATE       400000///* bps */
#define SPI1_BAUDRATE       400000///
void Bsp_SPI_Init(void)
{
	SysSpiInit(SCH_Spi0,SPI0_BAUDRATE);
	SysSpiInit(SCH_Spi1,SPI1_BAUDRATE);
}
////=================================================================================================================
void Bsp_CAN_Init(void)
{
	///SysCanInit(SCH_Can0);
}
////=================================================================================================================
void Bsp_ADC_Init(void)
{
	SysAdcInit();
}
////=================================================================================================================
void Bsp_I2C_Init(void)
{
	I2C0Init();
	I2C1Init();
}
////=================================================================================================================
void Bsp_FLASH_Init(void)
{
	SysFlashInit();
}



