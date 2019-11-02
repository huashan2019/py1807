///=========================
#ifndef __MW_UART_H
#define __MW_UART_H


#define UART0_FUNC    ENABLE
#define UART1_FUNC    ENABLE
#define UART2_FUNC    ENABLE
#define UART3_FUNC    DISABLE


#if 0/// UART2_FUNC == ENABLE
#define IO_UART2_LIN_EN                GPIOB
#define GPIO_UART2_LIN_EN              GPIO_PTH1//////
#define TurnOn_UART2_LIN_EN        	   GPIO_PinSet(GPIO_UART2_LIN_EN)
#define TurnOff_UART2_LIN_EN     	   GPIO_PinClear(GPIO_UART2_LIN_EN)
#define IO_UART2_RX_EN                 GPIOC
#define GPIO_UART2_RX_EN               GPIO_PTI0//////
#define IO_UART2_TX_EN                 GPIOC
#define GPIO_UART2_TX_EN               GPIO_PTI1//////
#endif
///=================================================

typedef enum
{
	SCH_Uart0,
	SCH_Uart1,
	SCH_Uart2,
	SCH_Uart3,
	SCH_Uart_Num
}Uart_T;

typedef enum
{
	Uart_Rx,
	Uart_Tx
}Uart_RT;
///============================================================================================
extern SCH_U16 UartTxCnt(Uart_T uart);
extern SCH_BOOL UartTxData(Uart_T uart, SCH_U8 *const data, SCH_U16 Len);
extern SCH_BOOL UartTxData_Direct(Uart_T uart, SCH_U8 *data, SCH_U16 Len);
extern SCH_U16 UartRxCnt(Uart_T uart);
extern SCH_BOOL UartRxData(Uart_T uart, SCH_U8 *data, SCH_U16 Len);
extern void SysUartExit(Uart_T uart);
extern SCH_BOOL SysUartInit(Uart_T uart,SCH_U32 Baudrate);
extern void UART_IntSerive(Uart_T uart);
#endif
