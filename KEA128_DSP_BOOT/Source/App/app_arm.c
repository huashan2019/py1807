/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_arm.c  ---   与ARM通讯
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"


Uart_T Uart_ARM = SCH_Uart_BT;
///===============================================
#define HEAD_ADDRESS_ARM     0xAA
#define HEAD_ADDRESS_MCU     0x55
typedef struct
{
	SCH_U8 ArmRxData[150];
	SCH_U8 ArmRxData_Length;
	SCH_U8 ArmRxData_Flag;
	SCH_U8 ArmTxData[32];
	SCH_U8 ArmTxData_Length;
	SCH_U8 ArmTxData_Parity;
	SCH_U8 ArmCheck_ResendCounte;
	SCH_U8 ArmCheck_ResendTimer;
	SCH_U8 ArmCheck_Busy;
	SCH_U8 ArmCheck_Ack;
}ARM_MODUEL_S;
ARM_MODUEL_S ArmModuel; 
#define ArmRx_Head1     ArmModuel.ArmRxData[0]
#define ArmRx_Head2     ArmModuel.ArmRxData[1]
#define ArmRx_Length    ArmModuel.ArmRxData[2]
#define ArmRx_Sequence  ArmModuel.ArmRxData[3]
#define ArmRx_GroupID   ArmModuel.ArmRxData[4]
#define ArmRx_SubID     ArmModuel.ArmRxData[5]
#define ArmRx_Data      ArmModuel.ArmRxData[6]
#define ArmTx_Head1     ArmModuel.ArmTxData[0]
#define ArmTx_Head2     ArmModuel.ArmTxData[1]
#define ArmTx_Length    ArmModuel.ArmTxData[2]
#define ArmTx_Sequence  ArmModuel.ArmTxData[3]
#define ArmTx_GroupID   ArmModuel.ArmTxData[4]
#define ArmTx_SubID     ArmModuel.ArmTxData[5]
#define ArmTx_Data      ArmModuel.ArmTxData[6]
///===============================================
void Arm_ACK(SCH_U8 ack_type)
{
	SCH_U8 buf[6];
	buf[0]=HEAD_ADDRESS_MCU;
	buf[1]=HEAD_ADDRESS_ARM;
	buf[2]=6;
	buf[3]=ArmRx_Sequence; 
	buf[4]=ack_type;
	buf[5]=GetCheckData_Xor(buf, 5);
	UartTxData(Uart_ARM, buf, 6);
}

void ArmDataAnalyse(void)
{
	SCH_U8 *pData=&ArmRx_Data;
	if(ArmRx_GroupID<COMM_NACK_NG)           
		Arm_ACK(COMM_ACK);
	switch(ArmRx_GroupID)
	{
		case A2M_ACK:///===========================================================================================ACK================
			if(ArmRx_Sequence==ArmTx_Sequence)
			{
				ArmModuel.ArmCheck_Ack=0;
				ArmModuel.ArmCheck_Busy=0;
			}
			break;
		case A2M_SYS_CMD:///===========================================================================================A2M_SYS_CMD 0x01================
			switch(ArmRx_SubID)
			{	
				case A2M_MCU_VER:
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_MCU_VER);
					break;
				case A2M_WORK_STATE:
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_WORK_STATE);
					break;				
				default:break;
			}
			break;
		case A2M_IAP_CMD:///===========================================================================================A2M_IAP_CMD 0x08================
			switch(ArmRx_SubID)
			{
				case A2M_IAP_REQ:
					if(pData[0]==0x00)
						Set_IAP_Mode;
					break;
				case A2M_IAP_DATA:
					if(Iap.IAP_Sn == (SCH_U16)(((SCH_U16)pData[0]<<8)+pData[1]))
					{
						sch_memcpy(Iap.IAP_Data,&pData[2],ArmRx_Length-9);
						Iap.IAP_DataOk = 1;
					}
					break;
				case A2M_IAP_C_L:
					Iap.IAP_Len = 0x00000000;
					Iap.IAP_Len |= pData[0]<<24;
					Iap.IAP_Len |= pData[1]<<16;
					Iap.IAP_Len |= pData[2]<<8;
					Iap.IAP_Len |= pData[3];
					Iap.IAP_Checksum = 0x00000000;
					Iap.IAP_Checksum |= pData[4]<<24;
					Iap.IAP_Checksum |= pData[5]<<16;
					Iap.IAP_Checksum |= pData[6]<<8;
					Iap.IAP_Checksum |= pData[7];
					break;
				case A2M_IAP_RESET:
					SCH_INT_DISABLE;
					SysFlashInit();
					Out_IapMode();
					BootloaderJumpToApp();
					while(1);
				default:break;
			}
			break;
		default:break;
	}
}

void ArmDataRxPro(void)
{
	while(UartRxCnt(Uart_ARM))
	{
		FeedDog();
		switch(ArmModuel.ArmRxData_Flag)
		{
			case 0:
				UartRxData(Uart_ARM, ArmModuel.ArmRxData, 1);
				if(ArmModuel.ArmRxData[0] != HEAD_ADDRESS_ARM)
					break;
				ArmModuel.ArmRxData_Flag = 1;
				break;
			case 1:
				UartRxData(Uart_ARM, &ArmModuel.ArmRxData[1], 1);
				if(ArmModuel.ArmRxData[1] != HEAD_ADDRESS_MCU)
				{
					ArmModuel.ArmRxData_Flag = 0;
					break;
				}
				ArmModuel.ArmRxData_Flag = 2;
				break;
			case 2:
				UartRxData(Uart_ARM, &ArmModuel.ArmRxData[2], 1);
				ArmModuel.ArmRxData_Length = ArmModuel.ArmRxData[2];
				if(ArmModuel.ArmRxData_Length >= 150||ArmModuel.ArmRxData_Length <= 3)///length 不超过30
				{
					ArmModuel.ArmRxData_Flag = 0;
					break;
				}
				ArmModuel.ArmRxData_Flag = 3;
				break;
			case 3:
				if(UartRxData(Uart_ARM, &ArmModuel.ArmRxData[3], ArmModuel.ArmRxData_Length-3)==FALSE)
					return;
				if(ArmModuel.ArmRxData[ArmModuel.ArmRxData_Length-1] == GetCheckData_Xor(ArmModuel.ArmRxData,ArmModuel.ArmRxData_Length-1))
				{
					ArmDataAnalyse();
				}
				ArmModuel.ArmRxData_Flag = 0;
				ArmModuel.ArmRxData_Length = 0;
				break;
			default:
				break;
		}
	}
}
///==============================================================================================================================
void M2A_TxService(void)
{
	MESSAGE pMsg;
	SCH_U8 *pData=&ArmTx_Data;
	SCH_U8 length_data = 0;
	SCH_U8 sub_id;
	if(FALSE==GetMessage(ARM_MODULE,&pMsg))
		return;
	sub_id=LSB(pMsg.prm);
	switch(pMsg.ID)
	{
		case M2A_SYS_CMD:
			switch(sub_id)
			{	
				case M2A_MCU_VER:
					length_data=sizeof(MCU_VERSION);
					sch_memcpy(pData,MCU_VERSION,length_data);
					break;
				case M2A_WORK_STATE:
					pData[length_data++]=0x01;
					break;					
				default:break;
			}
			break;			
		case M2A_IAP_CMD:
			switch(sub_id)
			{
				case M2A_IAP_REQ_C_L:
					pData[length_data++]=0x00;
					pData[length_data++]=0x00;
					break;
				case M2A_IAP_REQ_DATA:
					pData[length_data++]=(SCH_U8)(Iap.IAP_Sn>>8);
					pData[length_data++]=(SCH_U8)Iap.IAP_Sn;
					break;
				case M2A_IAP_REQ_RESET:
					pData[length_data++]=0x00;
					pData[length_data++]=0x00;
					break;
				default:break;
			}
			break;
		default:break;
	}
	ArmTx_Head1 = HEAD_ADDRESS_MCU;
	ArmTx_Head2 = HEAD_ADDRESS_ARM;
	ArmTx_Length = length_data+7;
	ArmTx_Sequence++;
	ArmTx_GroupID = pMsg.ID;
	ArmTx_SubID = sub_id;
	ArmModuel.ArmTxData_Parity = GetCheckData_Xor(ArmModuel.ArmTxData,ArmTx_Length-1);
	ArmModuel.ArmTxData[ArmTx_Length-1] = ArmModuel.ArmTxData_Parity;
	UartTxData(Uart_ARM, ArmModuel.ArmTxData, ArmTx_Length);
	ArmModuel.ArmCheck_Ack=1;
	ArmModuel.ArmCheck_ResendCounte=0;
	ArmModuel.ArmCheck_ResendTimer=0;
	ArmModuel.ArmCheck_Busy=1;
}
///==========================================================================================================================
void TASK_Arm_Pro(void)
{
	/*if(Get_OsStartOk==OFF||Get_SysPower_Flag==OFF)  
	{
		return;
	}
	if(ArmModuel.ArmCheck_Ack)
	{
		ArmModuel.ArmCheck_ResendTimer++;
		if(ArmModuel.ArmCheck_ResendTimer==T200MS_8)
		{  	
			ArmModuel.ArmCheck_ResendTimer=0;
			if(++ArmModuel.ArmCheck_ResendCounte<3)
			{
				 UartTxData(Uart_ARM, ArmModuel.ArmTxData, ArmTx_Length);
			}
			else
			{
				ArmModuel.ArmCheck_ResendCounte=0;
				ArmModuel.ArmCheck_Busy=0;
				ArmModuel.ArmCheck_Ack=0;
			}
		}
	}
	else if(!ArmModuel.ArmCheck_Busy)*/
	{
		M2A_TxService();
	}
}


