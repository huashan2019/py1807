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
Tanspond_A_B_Data TansData;
QUEUE_T SongName_QUEUE = {0,0,0,15,64,TansData.A2B_SongName};
///===============================================
#define HEAD_ADDRESS_ARM     0xAA
#define HEAD_ADDRESS_MCU     0x55
typedef struct
{
	SCH_U8 ArmRxData[150];
	SCH_U8 ArmRxData_Length;
	SCH_U8 ArmRxData_Flag;
	SCH_U8 ArmTxData[150];
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
///==========================================================================================ArmDataAnalyse====================
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
			if(ArmRx_SubID == 0x01)
			{
				Clr_ArmUpdata_Flag;
				PostMessage(ARM_MODULE,M2A_SYS_CMD,SCH_WORD(0x00,M2A_ARM_VER));
			}
			else if(ArmRx_SubID == 0x10)///升级中
			{
				Set_ArmUpdata_Flag;
			}
			else if(ArmRx_SubID == 0x11)///ARM版本号
			{
				TansData.ARM_Ver[0] = GetMin(30,ArmRx_Length-7);
				sch_memcpy(&TansData.ARM_Ver[1],&pData[0],TansData.ARM_Ver[0]);
				PostMessage(BT_MODULE,A2M_SYS_CMD,SCH_WORD(0x00,M2A_ARM_VER));
			}
			break;	
		case A2M_USB_VOL_INFO:
			if(ArmRx_SubID == 0x0d)
			{
				App_Dsp.Dsp_USB_Vol = pData[1] ;/*USB main vol 0~40*/
				App_Dsp.Dsp_USB_MAX_Vol = pData[2] ;
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x0D));
			}
			break;
		case A2M_SPDIF_INFO:
			switch(ArmRx_SubID)
			{
				case 0x01:///
					switch(pData[0])
					{
						case 0x00:
							break;
						case 0x01:///mute
							{
								SCH_U8 Mute = (App_Dsp.Dsp_Data.Mute[DSP_CHANNEL_ALL_NONE] == DSP_UNMUTE) ? DSP_MUTE : DSP_UNMUTE;
								Dsp_Mute(DSP_CHANNEL_ALL_NONE,Mute);
								PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x08));
							}
							break;
						case 0x02:///vol+
							VolData = GetMin(DSP_VOL_MAX,VolData + 1);
							Vol_Ctl(VolData);
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0D));
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x06));
							break;
						case 0x03:///vol-
							VolData = VolData ? VolData - 1 : 0;
							Vol_Ctl(VolData);
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0D));
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x06));
							break;
						case 0xF1:
							PostMessage(BT_MODULE,M2A_SPDIF_INFO,SCH_WORD(pData[0],ArmRx_SubID));
							break;
						default:break;
					}
					break;
				case 0x02:
					TansData.A2B_PlayPause_S = pData[0];
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(pData[0],ArmRx_SubID));
					break;
				case 0x03:
					TansData.A2B_Cycle_S = pData[0];
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(pData[0],ArmRx_SubID));
				case 0x04:
					sch_memcpy(TansData.A2B_Curr_T,  pData,   2);
					sch_memcpy(TansData.A2B_Total_T,&pData[2],2);
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x05:
					sch_memcpy(TansData.A2B_Total_T,pData,2);
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x06:
					TansData.A2B_CatalogueName[0] = GetMin(49,ArmRx_Length-7);
					sch_memcpy(&TansData.A2B_CatalogueName[1],pData,TansData.A2B_CatalogueName[0]);
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x07:
				case 0x87:
					TansData.A2B_TotalNum[0] = pData[0];
					TansData.A2B_TotalNum[1] = pData[1];
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x08:
					TansData.A2B_ID3_Song[0] = GetMin(49,ArmRx_Length-7);
					sch_memcpy(&TansData.A2B_ID3_Song[1],pData,TansData.A2B_ID3_Song[0]);
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x09:
					TansData.A2B_ID3_Singer[0] = GetMin(49,ArmRx_Length-7);
					sch_memcpy(&TansData.A2B_ID3_Singer[1],pData,TansData.A2B_ID3_Singer[0]);
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x0A:
					TansData.A2B_ID3_Album[0] = GetMin(49,ArmRx_Length-7);
					sch_memcpy(&TansData.A2B_ID3_Album[1],pData,TansData.A2B_ID3_Album[0]);
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				case 0x0B:
				case 0x8B:
					{
						SCH_U8 SongName[64];
						SongName[0] = pData[0];
						SongName[1] = pData[1];
						SongName[2] = GetMin(60,ArmRx_Length-9);
						sch_memcpy(&SongName[3],&pData[2],SongName[2]);
						if(Queue_In(&SongName_QUEUE,SongName,1))
							PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					}
					break;
				case 0x0C:
				case 0x8C:
					TansData.A2B_PlayNum[0] = pData[0];
					TansData.A2B_PlayNum[1] = pData[1];
					PostMessage(BT_MODULE,A2M_SPDIF_INFO,SCH_WORD(0x00,ArmRx_SubID));
					break;
				default:break;
			}
			break;
		case A2M_DSP_DATA_STORE:
			
			break;
		case A2M_IAP_CMD:///===========================================================================================A2M_IAP_CMD 0x08================
			if(ArmRx_SubID==0x01)
			{
				Set_Uart_Choose(UART_CHOOSE_PC1);
				Set_IAP_Mode;
			}
			break;
		default:break;
	}
}
void ArmDataRxPro(void)
{
	while(UartRxCnt(Uart_ARM))
	{
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
				if(ArmModuel.ArmRxData_Length > 150||ArmModuel.ArmRxData_Length < 3)///length 不超过40
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
	SCH_U8 sub_id,index;
	if(FALSE==GetMessage(ARM_MODULE,&pMsg))
		return;
	sub_id=LSB(pMsg.prm);
	index =MSB(pMsg.prm);
	switch(pMsg.ID)
	{
		case M2A_SYS_CMD:
			switch(sub_id)
			{
				case 0x01:
					pData[length_data++] = index;
					break;
				case 0x05:				
					pData[length_data++] = App_Dsp.Dsp_USB_Vol;/*USB main vol 0~40*/
					pData[length_data++] = App_Dsp.Dsp_USB_MAX_Vol;/*USB max*/
					pData[length_data++] = 0;/*mute state*/
					pData[length_data++] = 0;/*nav vol state*/
					pData[length_data++] = 0;/*vol state:0;media,1,nav*/
					break;
					
				default:break;
			}
			break;
		case M2A_SPDIF_INFO:
			switch(sub_id)
			{
				case 0x01:
					pData[length_data++] = index;
					if(index == 0x80)
					{
						pData[length_data++] = TansData.B2A_PlayNum[0];
						pData[length_data++] = TansData.B2A_PlayNum[1];
					}
					else if(index == 0x81)
					{
						pData[length_data++] = TansData.B2A_81Num[0];
						pData[length_data++] = TansData.B2A_81Num[1];
					}
					else if(index == 0x82)
					{
						pData[length_data++] = TansData.B2A_PlayTime[0];
						pData[length_data++] = TansData.B2A_PlayTime[1];
					}
					break;
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
				case 0x06:
				case 0x07:
				case 0x87:
				case 0x08:
				case 0x09:
				case 0x0A:
				case 0x0C:
				
				case 0x0E:/*VOL+*/
				case 0x0F:/*VOL-*/
				case 0x8C:
					pData[length_data++] = index;
					break;
				case 0x0B:
				case 0x8B:
					pData[length_data++] = TansData.B2A_NeedStartNum[0];
					pData[length_data++] = TansData.B2A_NeedStartNum[1];
					pData[length_data++] = TansData.B2A_NeedEndNum[0];
					pData[length_data++] = TansData.B2A_NeedEndNum[1];
					break;
				default:break;
			}
			break;
		case M2A_DSP_DATA_STORE:
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
	if(/*Get_OsStartOk==OFF||*/Get_SysPower_Flag==OFF)  
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
	else if(!ArmModuel.ArmCheck_Busy)
	{
		M2A_TxService();
	}
}


