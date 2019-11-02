/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_uart.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.05.05
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

#if UART0_FUNC == ENABLE
#define MAX_UART0_TX_BUF       256
SCH_U8 Uart0_TX_buf[MAX_UART0_TX_BUF];
QUEUE_T UART0_TX_QUEUE = {0,0,0,MAX_UART0_TX_BUF,1,Uart0_TX_buf};
#define MAX_UART0_RX_BUF       256
SCH_U8 Uart0_RX_buf[MAX_UART0_RX_BUF];
QUEUE_T UART0_RX_QUEUE = {0,0,0,MAX_UART0_RX_BUF,1,Uart0_RX_buf};
#endif
#if UART1_FUNC == ENABLE
#define MAX_UART1_TX_BUF       256
SCH_U8 Uart1_TX_buf[MAX_UART1_TX_BUF];
QUEUE_T UART1_TX_QUEUE = {0,0,0,MAX_UART1_TX_BUF,1,Uart1_TX_buf};
#define MAX_UART1_RX_BUF       256
SCH_U8 Uart1_RX_buf[MAX_UART1_RX_BUF];
QUEUE_T UART1_RX_QUEUE = {0,0,0,MAX_UART1_RX_BUF,1,Uart1_RX_buf};
#endif
#if UART2_FUNC == ENABLE
#define MAX_UART2_TX_BUF       256
SCH_U8 Uart2_TX_buf[MAX_UART2_TX_BUF];
QUEUE_T UART2_TX_QUEUE = {0,0,0,MAX_UART2_TX_BUF,1,Uart2_TX_buf};
#define MAX_UART2_RX_BUF       256
SCH_U8 Uart2_RX_buf[MAX_UART2_RX_BUF];
QUEUE_T UART2_RX_QUEUE = {0,0,0,MAX_UART2_RX_BUF,1,Uart2_RX_buf};
#endif
#if UART3_FUNC == ENABLE
#define MAX_UART3_TX_BUF       128
SCH_U8 Uart3_TX_buf[MAX_UART3_TX_BUF];
QUEUE_T UART3_TX_QUEUE = {0,0,0,MAX_UART3_TX_BUF,1,Uart3_TX_buf};
#define MAX_UART3_RX_BUF       128
SCH_U8 Uart3_RX_buf[MAX_UART3_RX_BUF];
QUEUE_T UART3_RX_QUEUE = {0,0,0,MAX_UART3_RX_BUF,1,Uart3_RX_buf};
#endif
QUEUE_T *const UartBufAddr[][2] = 
{
#if UART0_FUNC == ENABLE
	&UART0_RX_QUEUE,&UART0_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART1_FUNC == ENABLE
	&UART1_RX_QUEUE,&UART1_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART2_FUNC == ENABLE
	&UART2_RX_QUEUE,&UART2_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if UART3_FUNC == ENABLE
	&UART3_RX_QUEUE,&UART3_TX_QUEUE,
#else
	NULL,NULL
#endif
};

UART_Type *Uart_Arry[]={UART0,UART1,UART2};
///========================================================
void UartBufInit(Uart_T uart,Uart_RT TxRx)
{
	if(UartBufAddr[uart][TxRx])
		Queue_Init(UartBufAddr[uart][TxRx]);
}
SCH_U16 UartBufCnt(Uart_T uart,Uart_RT TxRx)
{
	QUEUE_T *pUartBuf;
	pUartBuf = UartBufAddr[uart][TxRx];
	if(pUartBuf)
		return Queue_Cnt(pUartBuf);
	return 0;
}
SCH_BOOL UartGetFromBuf(Uart_T uart, Uart_RT TxRx, SCH_U8 *data, SCH_U16 Len)
{
	QUEUE_T *pUartBuf;
	pUartBuf = UartBufAddr[uart][TxRx];
	if(pUartBuf)
		return Queue_Out(pUartBuf, data, Len);
	return FALSE;
}
SCH_BOOL UartPutToBuf(Uart_T uart, Uart_RT TxRx, SCH_U8 *const data, SCH_U16 Len)
{
	QUEUE_T *pUartBuf;
	pUartBuf = UartBufAddr[uart][TxRx];
	if(pUartBuf)
		return Queue_In(pUartBuf, data,Len);
	return FALSE;
}
/********************************************************************************
**  Function	: UartSendData8
**  Author		: wenjunHu
**  Created on	: 20160621
**  Description	: 发送一个8位的数据
**  Return		: void
********************************************************************************/
void UartSendData8(Uart_T uart,SCH_U8 u8data)
{
	UART_PutChar(Uart_Arry[uart], u8data); 
}
/********************************************************************************
**  Function	: UartTxInt En/Dis
**  Author		: wenjunHu
**  Created on	: 20160621
**  Description	:
**  Return		: void
********************************************************************************/
void UartTxIntEn(Uart_T uart)
{
	UART_EnableInterrupt(Uart_Arry[uart], UART_TxCompleteInt);/* Enable Tx interrupt */
}
void UartTxIntDis(Uart_T uart)
{
	UART_DisableInterrupt(Uart_Arry[uart], UART_TxCompleteInt);/* Disable Tx interrupt */
}
/********************************************************************************
**  Function	: UartRx/TxCnt
**  Author		: wenjunHu
**  Created on	: 20160621
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 UartRxCnt(Uart_T uart)
{
	return UartBufCnt(uart,Uart_Rx);
}
SCH_U16 UartTxCnt(Uart_T uart)
{
	return UartBufCnt(uart,Uart_Tx);
}
/********************************************************************************
**  Function    : Uart_Rx_DataPro
**  Author      : wenjunHu
**  Created on  : 20160621
**  Description	:  ---   int函数中执行
**  Return		: NULL
********************************************************************************/
void Uart_Rx_DataPro(Uart_T uart,SCH_U8 data)
{
	UartPutToBuf(uart,Uart_Rx,&data,1);
}
/********************************************************************************
**  Function    : Uart_Tx_DataPro
**  Author      : wenjunHu
**  Created on  : 20160621
**  Description :   ---   int函数中执行
**  Return      : NULL
********************************************************************************/
void Uart_Tx_DataPro(Uart_T uart)
{
	SCH_U8 u8data;
	if(UartTxCnt(uart) == 0)
	{
		UartTxIntDis(uart);
		return;
	}
	if(UartGetFromBuf(uart,Uart_Tx,&u8data,1))
	{
		UartSendData8(uart, u8data);
		UartTxIntEn(uart);
	}
}
/********************************************************************************
**  Function	: UartRxData
**  Author		: wenjunHu
**  Created on	: 20160621
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL UartRxData(Uart_T uart, SCH_U8 *data, SCH_U16 Len)
{
	return UartGetFromBuf(uart,Uart_Rx,data,Len);
}
/********************************************************************************
**  Function         : UartTxData
**  Author           : wenjunHu
**  Created on       : 20160621
**  Description      :
**  Return           : BOOL
********************************************************************************/
SCH_BOOL UartTxData(Uart_T uart, SCH_U8 *const data, SCH_U16 Len)
{
	if(UartTxCnt(uart) != 0)
	{
		if(UartPutToBuf(uart,Uart_Tx,data,Len))
			return TRUE;
		return FALSE;
	}
	if(UartPutToBuf(uart,Uart_Tx,data,Len))
	{
		Uart_Tx_DataPro(uart);
		return TRUE;
	}
	return FALSE;
}
SCH_BOOL UartTxData_Direct(Uart_T uart, SCH_U8 *data, SCH_U16 Len)
{
	while(Len--)
	{
		UartSendData8(uart,*data);
		data++;
	}
	return TRUE;
}

/********************************************************************************
**  Function    : UART_IntSerive
**  Author      : wenjunHu
**  Created on  : 20170407
**  Description :  ---   int函数中执行
**  Return      : void
********************************************************************************/
void UART_IntSerive(Uart_T uart)
{
	SCH_U8 data;
	if(UART_CheckFlag(Uart_Arry[uart], UART_FlagRDRF))/* Rx interrupt */
	{
		data = UART_GetChar(Uart_Arry[uart]);
		Uart_Rx_DataPro(uart, data);
	}
	if(UART_CheckFlag(Uart_Arry[uart], UART_FlagTC))/* Tx interrupt */
	{
		if(Uart_Arry[uart]->C2 & UART_C2_TCIE_MASK)
		{	
			Uart_Tx_DataPro(uart);
		}
	}
}

/********************************************************************************
**  Function    : SysUartExit
**  Author      : wenjunHu
**  Created on  : 20170830
**  Description :
**  Return      : BOOL
********************************************************************************/
void SysUartExit(Uart_T uart)
{
	switch(uart)
	{
		case SCH_Uart0:
#if UART0_FUNC == ENABLE
			UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
			UART_DisableInterrupt(Uart_Arry[uart], UART_RxBuffFullInt);
			UART_DisableInterrupt(Uart_Arry[uart], UART_TxCompleteInt);
			NVIC_DisableIRQ (UART0_IRQn);
			SIM->SCGC &= ~SIM_SCGC_UART0_MASK; 
#endif
			break;
		case SCH_Uart1:
#if UART1_FUNC == ENABLE
			UART1->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
			UART_DisableInterrupt(Uart_Arry[uart], UART_RxBuffFullInt);
			UART_DisableInterrupt(Uart_Arry[uart], UART_TxCompleteInt);
			NVIC_DisableIRQ(UART1_IRQn);
			SIM->SCGC &= ~SIM_SCGC_UART1_MASK;
#endif
			break;
		case SCH_Uart2:
#if UART2_FUNC == ENABLE
			UART2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
			UART_DisableInterrupt(Uart_Arry[uart], UART_RxBuffFullInt);
			UART_DisableInterrupt(Uart_Arry[uart], UART_TxCompleteInt);
			NVIC_DisableIRQ(UART2_IRQn);
			SIM->SCGC &= ~SIM_SCGC_UART2_MASK;
#endif
#if 0 ///UART2_FUNC == ENABLE
			TurnOff_UART2_LIN_EN;
			UART2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
			UART_DisableInterrupt(Uart_Arry[uart], UART_RxBuffFullInt);
			UART_DisableInterrupt(Uart_Arry[uart], UART_TxCompleteInt);
			UART_DisableInterrupt(Uart_Arry[uart], UART_IdleLineInt);
			NVIC_DisableIRQ(UART2_IRQn);
			SIM->SCGC &= ~SIM_SCGC_UART2_MASK;
#endif
			break;
		case SCH_Uart3:
			break;
		default:break;
	}
}
/********************************************************************************
**  Function    : SysUartInit
**  Author      : 
**  Created on  : 20170406
**  Description :
**  Return      : BOOL
********************************************************************************/
SCH_BOOL SysUartInit(Uart_T uart,SCH_U32 Baudrate)
{
	UART_ConfigType t_UART_Config;        
	t_UART_Config.u32Baudrate = Baudrate;
	t_UART_Config.u32SysClkHz = BUS_CLK_HZ;
	switch(uart)
	{
		case SCH_Uart0:
#if UART0_FUNC == ENABLE
			UART_Init(UART0, &t_UART_Config);
			UART_EnableInterrupt(UART0,UART_RxBuffFullInt);
			NVIC_EnableIRQ (UART0_IRQn);			
			NVIC_SetPriority(UART0_IRQn, (1<<__NVIC_PRIO_BITS) - 3);
#endif
			break;
		case SCH_Uart1:
#if UART1_FUNC == ENABLE
			SIM_RemapUART1ToPTC_6_7();
			UART_Init(UART1, &t_UART_Config);
			UART_EnableInterrupt(UART1,UART_RxBuffFullInt);
			NVIC_EnableIRQ(UART1_IRQn);			
			NVIC_SetPriority(UART1_IRQn, (1<<__NVIC_PRIO_BITS) - 3);
#endif
			break;
		case SCH_Uart2:	
#if UART2_FUNC == ENABLE
			SIM_RemapUART2ToPTD_6_7();
			UART_Init(UART2, &t_UART_Config);
			UART_EnableInterrupt(UART2,UART_RxBuffFullInt);
			NVIC_EnableIRQ(UART2_IRQn);			
			NVIC_SetPriority(UART2_IRQn, (1<<__NVIC_PRIO_BITS) - 3);
#endif
#if 0
			SIM_RemapUART2ToPTI_0_1();
			GPIO_PinInit(GPIO_UART2_RX_EN, GPIO_PinInput_InternalPullup);
			GPIO_PinInit(GPIO_UART2_LIN_EN, GPIO_PinOutput_HighCurrent);	
			TurnOn_UART2_LIN_EN;
			UART_Init(UART2, &t_UART_Config);
			UART_EnableInterrupt(UART2,UART_RxBuffFullInt);
			UART_EnableInterrupt(UART2,UART_IdleLineInt);
			UART2->S2 |= (UART_S2_BRK13_MASK|UART_S2_LBKDE_MASK);
			UART2->BDH |= UART_BDH_LBKDIE_MASK;
			NVIC_EnableIRQ (UART2_IRQn);
#endif
			break;
		case SCH_Uart3:
			break;
		default:
			return FALSE;
	}		 
	UartBufInit(uart, Uart_Tx);
	UartBufInit(uart, Uart_Rx);
	return TRUE;
}
///====================================================================================
