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


void SysHardware_Init(void)
{
	Bsp_Clock_Init();
	Bsp_SysTick_Init();
	Bsp_GPIO_Init();
	Bsp_WDOG_Init();  
	Bsp_UART_Init();
	Bsp_SPI_Init();
	Bsp_ADC_Init();
	Bsp_FLASH_Init();
	Print_Init();
}
void SysSoftware_Init(void)
{
	SysDataInit();
	DspInit();
	Check_Uart();
}
////============================================================================================================
SCH_U8 flag;
SCH_U8 reter;
SCH_U8 Data;

int main(void)
{
	SCH_INT_DISABLE;
	SysHardware_Init();
	SysSoftware_Init();
	Bsp_Relocation_Int();///Reset the interrupt vector table
	SCH_INT_ENABLE;
	Printf("APP Begin -- Software Version : %s \n", MCU_VERSION);
	Printf("Code generation time : %s %s \n", __DATE__, __TIME__);
	while(1)
	{
		FeedDog();
		SCH_Task_Execute();
		TASK_AccOff_Pro();
		BT_DataRxPro(Uart_CONNECT,&BtRxModuel);
		BT_DataRxPro(Uart_OTHER,  &PcRxModuel_0);
		///EepromDataRxPro();
		ArmDataRxPro();
		///if(flag == 1)
			///UartTxData(SCH_Uart0,"AT+PUBADDR=0\r",14);
		///if(flag == 2)
			///UartTxData(SCH_Uart0,"AT+PUBADDR?\r",13);
		///if(flag == 3)
			///UartTxData(SCH_Uart0,"AT+LBDADDR?\r",13);
		if(flag == 4)
			UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET));
		if(flag == 5)
			UartTxData(SCH_Uart_BT,"AT+NAME?\r",10);
		if(flag == 6)
			Data = AD1938_ReadData(reter);
		if(flag == 7)
			AD1938_WriteData(reter,Data);
		if(flag == 8)
			PostMessage(ARM_MODULE,M2A_SPDIF_INFO,SCH_WORD(0,2));
		if(flag == 9)
		{
			SCH_INT_DISABLE;
			default_download_IC_1();
			SCH_INT_ENABLE;
		}
		if(flag == 10)
		{
			SysUartInit(SCH_Uart1,115200);
		}
		flag = 0;
	}
}











