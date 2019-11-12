///=========================
#ifndef __MW_CONNECT_H
#define __MW_CONNECT_H
///===============================================
typedef struct
{
	SCH_U8 RxData[80];
	SCH_U8 RxData_Length;
	SCH_U8 RxData_Flag;
}Rx_MODUEL_S;
extern Rx_MODUEL_S      BtRxModuel;
extern Rx_MODUEL_S      PcRxModuel_0;
extern Rx_MODUEL_S      PcRxModuel_1;
///====================================================================================================
#define UART_CHOOSE_BT                  0xFFFFFFFF
#define UART_CHOOSE_PC                  0xAAAA5555
#define UART_CHOOSE_PC1                 0xAA55AA55
#define SCH_Uart_BT   SCH_Uart0  ///---  BT
#define SCH_Uart_PC   SCH_Uart2  ///SCH_Uart1  ///---  PC   
#define SCH_Uart_PC1  SCH_Uart2  ///SCH_Uart2  ///---  PC  
extern Uart_T Uart_CONNECT;
extern Uart_T Uart_OTHER;
extern Uart_T Uart_OTHER1;
///=================================================================================================
extern void Set_Uart_Choose(SCH_U32 DATA);
extern void Check_Uart(void);
extern void BT_DataRxPro(Uart_T Uart,Rx_MODUEL_S *RxModuel);
#endif
