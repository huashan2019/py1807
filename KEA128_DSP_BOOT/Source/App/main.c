/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : main.c
**  Description : This file is the application.
**  Author      : WenjunHu
**  Created on  : 2017.04.06
**  Note        : NULL
***************************************************************************************************
*/
	
#include "include.h"

///===============================================
#define UART_CHOOSE_ADDRESS             0x0001FE00
#define UART_CHOOSE_PC                  0xAAAA5555
#define UART_CHOOSE_PC1                 0xAA55AA55
SCH_BOOL Is_Uart_Choose_PC(void)
{
	return (*(SCH_U32 *)UART_CHOOSE_ADDRESS == UART_CHOOSE_PC) ? TRUE : FALSE;
}
void Set_Uart_Choose_PC(void)
{
	FLASH_EraseSector(UART_CHOOSE_ADDRESS);
	FLASH_Program1LongWord(UART_CHOOSE_ADDRESS, UART_CHOOSE_PC);
}
SCH_BOOL Is_Uart_Choose_PC1(void)
{
	return (*(SCH_U32 *)UART_CHOOSE_ADDRESS == UART_CHOOSE_PC1) ? TRUE : FALSE;
}
void Set_Uart_Choose_PC1(void)
{
	FLASH_EraseSector(UART_CHOOSE_ADDRESS);
	FLASH_Program1LongWord(UART_CHOOSE_ADDRESS, UART_CHOOSE_PC1);
}
void SET_Uart_Choose_BT(void)
{
	FLASH_EraseSector(UART_CHOOSE_ADDRESS);
}
///===============================================


////============================================================================================================
int main(void)
{
 	SCH_INT_DISABLE;
	if(!Is_IapMode())
	{
		BootloaderJumpToApp();
	}
	if(Is_ResetMode())///掉电升级===需要上电初始化
	{
		SystemInit_A();
		Bsp_Clock_Init();
		Bsp_SysTick_Init();
		Bsp_WDOG_Init();
		GPIO_PinInit(GPIO_BT_RESET_CTL, GPIO_PinOutput);
		GPIO_PinInit(GPIO_SYS_POWER_CTL, GPIO_PinOutput);
		LED_Init();
		SYS_Power_Ctl(ON);
		BT_RESET_RELEASE;
		SysWaitMs(100);
		Bsp_FLASH_Init();
	}
	In_ResetMode();
	if(Is_Uart_Choose_PC())
	{
		Uart_ARM = SCH_Uart2;
	}
	else if(Is_Uart_Choose_PC1())
	{
		Uart_ARM = SCH_Uart1;
	}
	Updata_Init();
	Bsp_UART_Init();
	Bsp_Relocation_Int();///Reset the interrupt vector table
	SCH_INT_ENABLE;
	Printf("APP Begin -- Software Version : %s \n", MCU_VERSION);
	Printf("Code generation time : %s %s \n", __DATE__, __TIME__);
	while(1)
	{
		FeedDog();
		MCU_Updata_pro();
		SCH_Task_Execute();
		ArmDataRxPro();///BT
	}
}











