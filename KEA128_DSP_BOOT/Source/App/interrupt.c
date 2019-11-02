



#include "include.h"

void KBI0_IRQHandler(void)
{
	NVIC_SystemReset();
}
void IRQ_IRQHandler(void)
{
	IRQ->SC |= IRQ_SC_IRQACK_MASK;
}

void ADC_IRQHandler(void)
{
	///ADC_ReadResultReg(ADC);
}

SCH_U32 TTT;
void SysTick_Handler(void)
{
	///if(Hero_Init_Timer)
		///Hero_Init_Timer--;
	Sys.TimeCounter++;
	/*F_1ms_Set;
	if(Sys.TimeCounter%2==0)
		F_2ms_Set;
	if(Sys.TimeCounter%4==0)*/
		F_4ms_Set;
	if(Sys.TimeCounter%2==0)
		F_8ms_Set;
	if(Sys.TimeCounter%4==0)
		F_16ms_Set;
	if(Sys.TimeCounter%8==0)
		F_32ms_Set;
	if(Sys.TimeCounter%25==0)
		F_100ms_Set;
	if(Sys.TimeCounter==3000)
		Sys.TimeCounter=0;
}

void RTC_IRQHandler(void)
{
	RTC_ClrFlags();
	GetCurrentTime();
}

void UART0_IRQHandler(void)
{
#if UART0_FUNC == ENABLE
	UART_IntSerive(SCH_Uart0);
#endif
}

void UART1_IRQHandler(void)
{
#if UART1_FUNC == ENABLE
	UART_IntSerive(SCH_Uart1);
#endif
}

void UART2_IRQHandler(void)
{
	
#if UART2_FUNC == ENABLE
		UART_IntSerive(SCH_Uart2);
#endif

#if 0/// UART2_FUNC == ENABLE
	if(UART_CheckFlag(UART2, UART_FlagLBKDIF))
	{
		UART2->S2 |= UART_S2_LBKDIF_MASK;
		UART2->S2 &= ~(UART_S2_LBKDE_MASK);
	}
	if(UART_CheckFlag(UART2, UART_FlagIDLE))
	{
		UART2->S2 |= UART_S2_LBKDE_MASK;
		UART_ReadDataReg(UART2);
	}
	UART_IntSerive(SCH_Uart2);
#endif
}


#if 0
void MSCAN_RX_IRQHandler(void)
{
	MSCAN_FrameType MSCAN_Data;
	Can_Data_T data = {0};
	if(CAN_ReadOneFrameFromBuff(MSCAN, &MSCAN_Data))
	{
		data.IDE = MSCAN_Data.bIsExtOrStand;
		if(data.IDE == FrameType_Ext)
			data.Id = MSCAN_Data.ID_Type.ID&0x1FFFFFFF;
		else
			data.Id = MSCAN_Data.ID_Type.ID&0x000007FF;
		data.RTR = MSCAN_Data.bIsRemoteFrame;
		data.DLC = MSCAN_Data.DLR;
		sch_memcpy(&data.Data,&MSCAN_Data.DSR,8);
		Can_Rx_DataPro(SCH_Can0,&data);
	}
}
void MSCAN_TX_IRQHandler (void)
{
	if(CAN_IsOverRunFlag (MSCAN))          /* OverRun flag */
	{
		CAN_ClearOVRIF_Flag (MSCAN);       /* Clear flag */
	}
	if(CAN_IsWakeUpIntFlag (MSCAN))       /* Wake up flag */
	{
		CAN_ClearWUPIF_Flag (MSCAN);       /* Clear flag */
	}
	if(CAN_IsStatusChangeFlag (MSCAN))    /* Status change flag */
	{
		CAN_ClearCSCIF_Flag (MSCAN);     /* Clear flag */
		CAN_GetReceiverStatus (MSCAN);     /* Get currently status */
		CAN_GetReceiveErrorCount (MSCAN);  /* Error count */
	}
	Can_Tx_DataPro(SCH_Can0);
}
#endif



