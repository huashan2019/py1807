/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_can.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/

#include "include.h"

#if CAN0_FUNC == ENABLE
#define MAX_CAN0_TX_BUF       10
Can_Data_T Can0_TX_buf[MAX_CAN0_TX_BUF];
QUEUE_T CAN0_TX_QUEUE = {0,0,0,MAX_CAN0_TX_BUF,sizeof(Can_Data_T),Can0_TX_buf};
#define MAX_CAN0_RX_BUF       10
Can_Data_T Can0_RX_buf[MAX_CAN0_RX_BUF];
QUEUE_T CAN0_RX_QUEUE = {0,0,0,MAX_CAN0_RX_BUF,sizeof(Can_Data_T),Can0_RX_buf};
#endif

QUEUE_T *const CanBufAddr[][2] = 
{
#if CAN0_FUNC == ENABLE
	&CAN0_RX_QUEUE,&CAN0_TX_QUEUE,
#else
	NULL,NULL,
#endif
};
MSCAN_Type *Can_Arry[]={MSCAN};
///===================================================================================================Filter=========
void Can0_Filter_Config(Can_Filter_C *pCanFilter)
{
	SCH_U32 u32IDAR0,u32IDMR0,u32IDAR1,u32IDMR1;
	u32IDAR0 = 0x0220
		      |0x0221
		      |0x0224;
	u32IDMR0 = (0x0220^0x0221)
		      |(0x0220^0x0224);
	(*pCanFilter).u32IDAR0 = u32IDAR0<<21|0x00000000;
	(*pCanFilter).u32IDMR0 = u32IDMR0<<21|0x0017FFFE;
	u32IDAR1 = 0x1880D3D0
		      |0x1890D3D0
		      |0x00001314;
	u32IDMR1 = (0x1880D3D0^0x1890D3D0)
		      |(0x1880D3D0^0x00001314);
	(*pCanFilter).u32IDAR1 = ((u32IDAR1<<1)&0x0007FFFE)|((u32IDAR1<<3)&0xFFE00000)|0x00080000;
	(*pCanFilter).u32IDMR1 = ((u32IDMR1<<1)&0x0007FFFE)|((u32IDMR1<<3)&0xFFE00000)|0x00100000;
}
///========================================================
void CanBufInit(Can_T can,Can_RT TxRx)
{
	if(CanBufAddr[can][TxRx])
		Queue_Init(CanBufAddr[can][TxRx]);
}
SCH_U16 CanBufCnt(Can_T can,Can_RT TxRx)
{
	QUEUE_T *pCanBuf;
	pCanBuf = CanBufAddr[can][TxRx];
	if(pCanBuf)
		return Queue_Cnt(pCanBuf);
	return 0;
}
SCH_BOOL CanGetFromBuf(Can_T can, Can_RT TxRx, Can_Data_T *data, SCH_U16 Len)
{
	QUEUE_T *pCanBuf;
	pCanBuf = CanBufAddr[can][TxRx];
	if(pCanBuf)
		return Queue_Out(pCanBuf, data, Len);
	return FALSE;
}
SCH_BOOL CanPutToBuf(Can_T can, Can_RT TxRx, Can_Data_T *const data, SCH_U16 Len)
{
	QUEUE_T *pCanBuf;
	pCanBuf = CanBufAddr[can][TxRx];
	if(pCanBuf)
		return Queue_In(pCanBuf, data, Len);
	return FALSE;
}
/********************************************************************************
**  Function	: CanRxCnt
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 CanRxCnt(Can_T can)
{
	return CanBufCnt(can,Can_Rx);
}
/********************************************************************************
**  Function	: CanTxCnt
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 CanTxCnt(Can_T can)
{
	return CanBufCnt(can,Can_Tx);
}
/********************************************************************************
**  Function	: CanRxData
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL CanRxData(Can_T can, Can_Data_T *data, SCH_U16 Len)
{
	return CanGetFromBuf(can,Can_Rx,data,Len);
}
/********************************************************************************
**  Function	: CanTxData
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL CanTxData(Can_T can, Can_Data_T *const data, SCH_U16 Len)
{
	if(CanTxCnt(can) != 0)
	{
		if(CanPutToBuf(can,Can_Tx,data,Len))
			return TRUE;
		return FALSE;
	}
	if(CanPutToBuf(can,Can_Tx,data,Len))
	{
		Can_Tx_DataPro(can);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************************
**  Function	: Can_Rx_DataPro
**  Author		: 
**  Created on	: 20161101
**  Description	:
**  Return		: NULL
********************************************************************************/
void Can_Rx_DataPro(Can_T can,Can_Data_T *const data)
{
	CanPutToBuf(can,Can_Rx,data,1);
}
/********************************************************************************
**  Function	: CAN_Transmit_Data
**  Author		: 
**  Created on	: 20170831
**  Description	: can数据类型转换成可发送的并发送
**  Return		: NULL
********************************************************************************/
void CAN_Transmit_Data(Can_T can,Can_Data_T *data)
{
	IDR1_3_UNION sIDR1;
	IDR1_3_UNION sIDR3;
	MSCAN_RegisterFrameType MSCAN_Data;
	if(data->IDE == FrameType_Ext)
	{
		sIDR1.IDR1.EID20_18_OR_SID2_0 = (data->Id>>18)&0x7;
		sIDR1.IDR1.R_TSRR = 1;
		sIDR1.IDR1.R_TEIDE = 1;
		sIDR1.IDR1.EID17_15 = (data->Id>>15)&0x7;
		sIDR3.IDR3.EID6_0 = data->Id&0x7F;
		sIDR3.IDR3.ERTR = data->RTR ? 1 : 0;
		MSCAN_Data.EIDR0 = (data->Id>>21)&0xFF;
		MSCAN_Data.EIDR1 = sIDR1.Bytes;
		MSCAN_Data.EIDR2 = (data->Id>>7)&0xFF;
		MSCAN_Data.EIDR3 = sIDR3.Bytes;
	}
	else
	{
		sIDR1.IDR1.EID20_18_OR_SID2_0 = data->Id&0x7;
		sIDR1.IDR1.R_TSRR = 0;
		sIDR1.IDR1.R_TEIDE = 0;
		sIDR1.IDR1.EID17_15 = 0x00;
		sIDR3.IDR3.EID6_0 = 0x00;
		sIDR3.IDR3.ERTR = data->RTR ? 1 : 0;
		MSCAN_Data.EIDR0 = (data->Id>>3)&0xFF;
		MSCAN_Data.EIDR1 = sIDR1.Bytes;
		MSCAN_Data.EIDR2 = 0x00;
		MSCAN_Data.EIDR3 = sIDR3.Bytes;
	}
	sch_memcpy(&MSCAN_Data.EDSR,&data->Data,8);
	MSCAN_Data.DLR = data->DLC;
	MSCAN_Data.BPR = 0x80; 
	while(!CAN_LoadOneFrameToBuff(Can_Arry[can], &MSCAN_Data));
}
/********************************************************************************
**  Function	: Can_Tx_DataPro
**  Author		: 
**  Created on	: 20161101
**  Description	: 
**  Return		: NULL
********************************************************************************/
void Can_Tx_DataPro(Can_T can)
{	
	Can_Data_T data;
	if(CanTxCnt(can) == 0)
	{
		CAN_TransmitterEmptyIntDisable(Can_Arry[can]);
		return;
	}
	if(CanGetFromBuf(can,Can_Tx,&data,1))
	{
		CAN_Transmit_Data(can,&data);
		CAN_TransmitterEmptyIntEn(Can_Arry[can]);
	}
}
/********************************************************************************
**  Function	: SysCanExit
**  Author		: wenjunHu
**  Created on	: 20170901
**  Description	:
**  Return		: void
********************************************************************************/
void SysCanExit(Can_T can)
{
	switch(can)
	{
		case SCH_Can0:
#if CAN0_FUNC == ENABLE
			TurnOff_CAN0_STD;
			CAN_DeInit(Can_Arry[can]);
			break;
#endif
		default:break;
	}
}
/********************************************************************************
**  Function	: SysCanInit
**  Author		: 
**  Created on	: 20170407
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL SysCanInit(Can_T can)
{
	MSCAN_ConfigType t_MSCAN_Config;
	Can_Filter_C Can_Filter={0};
	switch(can)
	{
		case SCH_Can0:
#if CAN0_FUNC == ENABLE
			GPIO_PinInit(GPIO_CAN0_STD, GPIO_PinOutput_HighCurrent);///STD
			TurnOn_CAN0_STD;
			SIM_RemapMSCANToPTH_2_7();
			Can0_Filter_Config(&Can_Filter);
			t_MSCAN_Config.sBaudRateSetting.SAMP     = BAUD_RATE_SAMP;///波特率 > 100K ---0   波特率 < 100K  ---1
			t_MSCAN_Config.sBaudRateSetting.BRP      = 3;
			t_MSCAN_Config.sBaudRateSetting.SJW      = SJW_1TQ;
			t_MSCAN_Config.sBaudRateSetting.TSEG1    = TSEG_7;
			t_MSCAN_Config.sBaudRateSetting.TSEG2    = TSEG_2;
			///20M/(3+1)/1/(7+2+1)=500K --- (TSEG1+1)/(TSEG1+TSEG2+1) = (7+2)/(7+2+1) = 90%;
			t_MSCAN_Config.u32IDAR0                  = Can_Filter.u32IDAR0;
			t_MSCAN_Config.u32IDAR1                  = Can_Filter.u32IDAR1;
			t_MSCAN_Config.u32IDMR0                  = Can_Filter.u32IDMR0;
			t_MSCAN_Config.u32IDMR1                  = Can_Filter.u32IDMR1;
			t_MSCAN_Config.u8IDARMode                = ID_ACCEPT_MODE_TWO32;
			t_MSCAN_Config.sSetting.bCanEn           = 1;
			t_MSCAN_Config.sSetting.bCLKSRC          = 1;
			t_MSCAN_Config.sSetting.bListenModeEn    = 0;
			t_MSCAN_Config.sSetting.bWakeUpEn 		 = 1;
			t_MSCAN_Config.sSetting.bLoopModeEn      = 0;
			t_MSCAN_Config.sSetting.bTimerEn         = 1;
			t_MSCAN_Config.sSetting.bWakeUpIEn       = 1;
			t_MSCAN_Config.sSetting.bRxFullIEn       = 1;
			t_MSCAN_Config.sSetting.bOverRunIEn      = 1;
			t_MSCAN_Config.sSetting.bStatusChangeIEn = 1;
			t_MSCAN_Config.sSetting.bTxEmptyIEn      = 1;
			CAN_Init(Can_Arry[can], &t_MSCAN_Config);
#endif
			break;
		default:
			return FALSE;
	}
	CanBufInit(can, Can_Tx);
	CanBufInit(can, Can_Rx);
	return TRUE;
}



