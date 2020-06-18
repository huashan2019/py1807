/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_bt.c  ---   与BT通讯
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
SCH_U8 WHEEL_CTL_Flag[3];

typedef struct
{
	SCH_U8 TxData[80];
	SCH_U8 TxData_Length;
	SCH_U8 TxData_Parity;
	SCH_U8 Check_ResendCounte;
	SCH_U8 Check_ResendTimer;
	SCH_U8 Check_Busy;
	SCH_U8 Check_Ack;
}BT_Tx_MODUEL_S;
BT_Tx_MODUEL_S BtTxModuel;
Rx_MODUEL_S      BtRxModuel;
Rx_MODUEL_S      PcRxModuel_0;
Rx_MODUEL_S      PcRxModuel_1;
#define BtRx_Head1     BtRxModuel.RxData[0]
#define BtRx_Head2     BtRxModuel.RxData[1]
#define BtRx_Length    BtRxModuel.RxData[2]
#define BtRx_Sequence  BtRxModuel.RxData[3]
#define BtRx_GroupID   BtRxModuel.RxData[4]
#define BtRx_SubID     BtRxModuel.RxData[5]
#define BtRx_Data      BtRxModuel.RxData[6]
#define BtTx_Head1     BtTxModuel.TxData[0]
#define BtTx_Head2     BtTxModuel.TxData[1]
#define BtTx_Length    BtTxModuel.TxData[2]
#define BtTx_Sequence  BtTxModuel.TxData[3]
#define BtTx_GroupID   BtTxModuel.TxData[4]
#define BtTx_SubID     BtTxModuel.TxData[5]
#define BtTx_Data      BtTxModuel.TxData[6]
///======================================================================================
SCH_U8 set_query_password;
#define PasswordCnt    6
const SCH_U8 DefaultPassword[PasswordCnt] = {0x32,0x35,0x37,0x30,0x38,0x39};
SCH_BOOL Is_PASSWORD(SCH_U8 *Data)
{
	SCH_U8 password[PasswordCnt];
	if(sch_compare(Data,DefaultPassword,PasswordCnt)==TRUE)
		return TRUE;
	Flash_Quick_RD_U8(FLASH_DATA_PASSWORD, password, PasswordCnt);
	if(sch_compare(Data,password,PasswordCnt)==TRUE)
		return TRUE;
	return FALSE;
}
SCH_BOOL Set_PASSWORD(SCH_U8 *Data)
{
	return Flash_Quick_WR_U8(FLASH_DATA_PASSWORD, Data, PasswordCnt);
}
///======================================================================================
void Bt_ACK(SCH_U8 ack_type)
{
	SCH_U8 buf[6];
	buf[0]=HEAD_ADDRESS_MCU;
	buf[1]=HEAD_ADDRESS_BT;
	buf[2]=6;
	buf[3]=BtRx_Sequence; 
	buf[4]=ack_type;
	buf[5]=GetCheckData_Xor(buf, 5);
	UartTxData(Uart_CONNECT, buf, 6);
}
/********************************************************************************
**  Function	: BtDataAnalyse
**  Author		: 
**  Created on	: 20190402
**  Description	:
**  Return		: 
********************************************************************************/
void BtDataAnalyse(void)
{
	SCH_U8 *pData=&BtRx_Data;
	if(BtRx_GroupID<COMM_NACK_NG)           
		Bt_ACK(COMM_ACK);
	switch(BtRx_GroupID)
	{
		case A2M_ACK:///===========================================================================================ACK================
			if(BtRx_Sequence==BtTx_Sequence)
			{
				BtTxModuel.Check_Ack=0;
				BtTxModuel.Check_Busy=0;
			}
			break;
		case A2M_WHEEL_CTL:///蓝牙方控按键
			if(BtRx_SubID == 0x00)
			{
				/*static SCH_U8 index;
				if(pData[0] == index)
					break;
				index = pData[0];*/
				WHEEL_CTL_Flag[0] = 1;
				WHEEL_CTL_Flag[1] = pData[1];
				WHEEL_CTL_Flag[2] = pData[0];
			}
			break;
		case A2M_SYS_CMD:
			switch(BtRx_SubID)
			{
				case A2M_MCU_VER:
					PostMessage(BT_MODULE, M2A_SYS_CMD, M2A_MCU_VER);
					break;
				case A2M_ARM_VER:
					PostMessage(BT_MODULE, M2A_SYS_CMD, M2A_ARM_VER);
					PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_ARM_VER);
					break;
				default:break;
			}
			break;
		case B2M_DSP_DATA:///A2M_DSP_DATA
			switch(BtRx_SubID)
			{
				case 0x00:
					if(App_Dsp.DSP_Updata_State == UpData_Idle)
					{
						if(pData[0]==0x00)
						{
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,BtRx_SubID));							
						} 
						else if(pData[0]==0x01)
						{
							if(!Get_DSP_OFF_Flag)
							{
								App_Dsp.DSP_Updata_State = Curr_TO_Flash;
								App_Dsp.DspUpdataNum = pData[1];
							}
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,BtRx_SubID));							
						}
						else if(pData[0]==0x02)
						{
							if(!Get_DSP_OFF_Flag)
							{
								App_Dsp.DSP_Updata_State = Flash_TO_Curr;
								App_Dsp.DspUpdataNum = pData[1];
							}
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x02,BtRx_SubID));							
						}
					}
					else
					{
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,BtRx_SubID));
					}
					break;
				case 0x01:///DSP状态
					if(pData[0]==0)
					{
						PostMessage(BT_MODULE,M2B_DSP_DATA,0x01);
					}
					else if(pData[0]==1)
					{
						if(!Get_DSP_OFF_Flag)
							Dsp_Data_Reset();
					}
					else if(pData[0]==2)
					{
						if(pData[1])
						{
							if(!Get_DSP_OFF_Flag)
								Dsp_EQ_Reset(pData[1]);
							if(pData[1] % 2)
								App_Dsp.RequestEQ_Left = 0x8000|((pData[1]-1)*32);
							else
								App_Dsp.RequestEQ_Right = 0x8000|((pData[1]-1)*32);
						}
					}
					else if(pData[0]==3)
					{
						if(pData[1])
						{
							if(!Get_DSP_OFF_Flag)
								Dsp_EQ_Direct(pData[1],SCH_ENABLE);
							if(pData[1] % 2)
								App_Dsp.RequestEQ_Left = 0x8000|((pData[1]-1)*32);
							else
								App_Dsp.RequestEQ_Right = 0x8000|((pData[1]-1)*32);
						}
					}
					else if(pData[0]==4)
					{
						if(pData[1])
						{
							if(!Get_DSP_OFF_Flag)
								Dsp_Filter_Reset(pData[1]);
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD((pData[1]-1)*HPLP_FILTER_CNT,0x03));
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD((pData[1]-1)*HPLP_FILTER_CNT+1,0x03));
						}
					}
					else if(pData[0]==5)
					{
						if(pData[1])
						{
							if(!Get_DSP_OFF_Flag)
								Dsp_EQ_Direct(pData[1],SCH_DISABLE);
							if(pData[1] % 2)
								App_Dsp.RequestEQ_Left = 0x8000|((pData[1]-1)*32);
							else
								App_Dsp.RequestEQ_Right = 0x8000|((pData[1]-1)*32);
						}
					}
					else if(pData[0]==6)
					{
						Dsp_TestMode();
					}
					break;
				case 0x02:///混音状态
					if(!Get_DSP_OFF_Flag)
						Dsp_Mix_Mixer(pData[0],&pData[2]);
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x82:///请求混音状态
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x02));
					break;
				case 0x03:///Filters
					if(!Get_DSP_OFF_Flag)
					{
						Filters_T Filter;
						Filter.Other.byte = pData[1];
						Filter.MODE0 = (Filter_MODE0)pData[2];
						Filter.MODE1 = (Filter_MODE1)pData[3];
						Filter.Freq = (pData[4]<<24)+(pData[5]<<16)+(pData[6]<<8)+pData[7];
						Filter.Gain = (pData[8]<<8)+pData[9];
						Dsp_GEN_Filter(pData[0]/HPLP_FILTER_CNT,(Filter_HPLP)(pData[0]%HPLP_FILTER_CNT),&Filter);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x83:///请求Filters
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x03));
					break;
				case 0x04:///EQ
					if(!Get_DSP_OFF_Flag)
					{
						EQ_T EQ;
						EQ.Other.byte = pData[1];
						EQ.MODE0    = (EQ_MODE0)pData[2];
						EQ.Boost    = (pData[3]<<8 )+pData[4];
						EQ.Freq     = (pData[5]<<24)+(pData[6]<<16)+(pData[7]<<8)+pData[8];
						EQ.Gain     = (pData[9]<<8 )+pData[10];
						EQ.Q_Factor = (pData[11]<<8)+pData[12];
						Dsp_EQ_Set(pData[0]/EQ_NUM_CNT,(EQ_NUM_T)(pData[0]%EQ_NUM_CNT),&EQ);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x84:///请求EQ
					if(pData[0]%32==31)
					{
						if(pData[0]%64 < 32)
							App_Dsp.RequestEQ_Left = 0x8000|(pData[0]/32*32);
						else
							App_Dsp.RequestEQ_Right = 0x8000|(pData[0]/32*32);
					}
					else
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x04));
					break;
				case 0x05:///Delay
					if(!Get_DSP_OFF_Flag)
					{
						Dsp_Delay(pData[0],(pData[1]<<8)+pData[2]);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x85:///请求Delay
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x05));
					break;
				case 0x06:///Single
					if(!Get_DSP_OFF_Flag)
					{
						if((pData[0] == DSP_CHANNEL_ALL_NONE)/*&&(Uart_CONNECT == SCH_Uart_PC)*/)
							App_Dsp.Dsp_Data.SingleMaxData = (pData[3]<<8)+pData[4];
						Dsp_Single(pData[0],(pData[1]<<8)+pData[2]);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					if(pData[0] == DSP_CHANNEL_ALL_NONE)
						CheckVol();
					break;
				case 0x86:///请求Single
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x06));
					break;
				case 0x07:///output
					if(!Get_DSP_OFF_Flag)
					{
						Dsp_OutPutChl(pData[0],pData[1]);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x87:///请求output
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x07));
					break;
				case 0x08:///mute
					if(!Get_DSP_OFF_Flag)
					{
						Dsp_Mute(pData[0],pData[1]);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x88:///请求mute
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x08));
					break;
				case 0x09:///ReName
					if(!Get_DSP_OFF_Flag)
					{
						Dsp_ReName(pData[0],&pData[1]);
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x89:///请求ReName
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x09));
					break;
				case 0x0A:///联调
					App_Dsp.DspUniteChannel0 = pData[0];
					App_Dsp.DspUniteChannel1 = pData[1];
					if(Dsp_Unite(pData[0],pData[1]))
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,BtRx_SubID));
					else
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x02,BtRx_SubID));
					break;
				case 0x0B:///密码
					if(pData[0] == 0x01)
					{
						set_query_password = pData[0];
						if(Is_PASSWORD(&pData[1]))
						{
							if(Set_PASSWORD(&pData[7]))
								PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,BtRx_SubID));
							else
								PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x03,BtRx_SubID));
						}
						else
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x02,BtRx_SubID));
					}
					else if(pData[0] == 0x02)
					{
						set_query_password = pData[0];
						if(Is_PASSWORD(&pData[1]))
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,BtRx_SubID));
						else
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x02,BtRx_SubID));
					}
					break;
				case 0x0C:///bt info
					if(pData[0] == 0x01)///addr
					{
						if(Uart_CONNECT == SCH_Uart_BT)
							PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0C));
						else
							UartTxData(SCH_Uart_BT,BT_LBDADDR_GET,sizeof(BT_LBDADDR_GET));
					}
					break;
				case 0x0D:///音量条
					if(pData[0] == 0x01)///总音量条0~40
					{
						if(!Get_DSP_OFF_Flag)
						{
							Vol_Ctl(pData[1]);
						}
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x06));
					}
					else if(pData[0] == 0x02)///USB音量条0~40
					{
					#if 1
						App_Dsp.Dsp_USB_Vol = pData[1];	
						App_Dsp.Dsp_USB_MAX_Vol = pData[2]; 
						PostMessage(ARM_MODULE,M2A_SYS_CMD,SCH_WORD(0x00,0x05));
					#else
						Dsp_USB_Pro(pData[1]);
						App_Dsp.Dsp_USB_Vol = pData[1];
						PostMessage(ARM_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
						PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					#endif
					}
					break;
				case 0x8D:///请求音量条
					//PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x0D));
					PostMessage(ARM_MODULE,M2A_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x0E:
					if(pData[0]==0x01)///DSP OFF
					{
						Dsp_OFF();
						Set_DSP_OFF_Flag;
					}
					else if(pData[0]==0x02)///DSP ON
					{
						Dsp_ON();
						Clr_DSP_OFF_Flag;
					}
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],BtRx_SubID));
					break;
				case 0x8E:
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(pData[0],0x0E));
				default:break;
			}
			break;
		case B2M_DSP_UPDATA:
			switch(BtRx_SubID)
			{
				case 0x00:
					if(pData[0]==0x01)
					{
						if((App_Dsp.DSP_Updata_State == UpData_Idle)&&(pData[1]!=0))
						{
							App_Dsp.DspUpdataNum = pData[1];
							App_Dsp.DSP_Updata_State = Flash_TO_File;
						}
						else
						{
							PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(0xFF,0x00));///ERROR
						}
					}
					else if(pData[0]==0x02)
					{
						if((App_Dsp.DSP_Updata_State == UpData_Idle)&&(pData[1]!=0))
						{
							App_Dsp.DspUpdataNum = pData[1];
							App_Dsp.DSP_Updata_State = File_TO_Flash;
						}
						else
						{
							PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(0xFF,0x00));///ERROR
						}
					}
					else if(pData[0]==0x81)
					{
						App_Dsp.DspUpdataNum = 0x00;
						App_Dsp.DspUpdataLen = 0x00000000;
						App_Dsp.DspUpdataSn  = 0x00;
						App_Dsp.DspUpdataCnt = 0x00;
						App_Dsp.DspUpdata_DataOk  = 0x00;
						App_Dsp.DspUpdataChecksum = 0x00;
					}
					break;
				case 0x01:///请求长度
					if((App_Dsp.DSP_Updata_State == Flash_TO_File)&&(pData[0] == App_Dsp.DspUpdataNum))
					{
						App_Dsp.DspUpdataLen = DSP_SPI_DATA_LENGTH;
						PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,BtRx_SubID));
					}
					break;
				case 0x02:///请求包数据
					if((App_Dsp.DSP_Updata_State == Flash_TO_File)&&(pData[0] == App_Dsp.DspUpdataNum))
					{
						App_Dsp.DspUpdataSn = pData[1];
						if((App_Dsp.DspUpdataSn+1) >= (App_Dsp.DspUpdataLen+63)/64)
							App_Dsp.DspUpdataCnt = App_Dsp.DspUpdataLen%64;
						else
							App_Dsp.DspUpdataCnt = 64;
						PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,BtRx_SubID));
					}
					break;
				case 0x03:///回应长度和校验
					if((App_Dsp.DSP_Updata_State == File_TO_Flash)&&(pData[0] == App_Dsp.DspUpdataNum))
					{
						App_Dsp.DspUpdataLen      = (pData[1]<<24)+(pData[2]<<16)+(pData[3]<<8)+pData[4];
						App_Dsp.DspUpdataChecksum = (pData[5]<<24)+(pData[6]<<16)+(pData[7]<<8)+pData[8];
					}
					break;
				case 0x04:///发送报数据
					if((App_Dsp.DSP_Updata_State == File_TO_Flash)&&(pData[0] == App_Dsp.DspUpdataNum))
					{
						if(pData[1] == App_Dsp.DspUpdataSn)
						{
							SCH_U8 *UpData = (SCH_U8 *)(&App_Dsp.Dsp_Data);
							UpData += (64*App_Dsp.DspUpdataSn);
							sch_memcpy(UpData,&pData[2],BtRx_Length-9);
							App_Dsp.DspUpdata_DataOk = 1;
						}
					}
					break;
				default:break;
			}
			break;
		case B2M_SPDIF_INFO:
			switch(BtRx_SubID)
			{
				case 0x01:
					if(pData[0]==0x80)
					{
						TansData.B2A_PlayNum[0] = pData[1];
						TansData.B2A_PlayNum[1] = pData[2];
					}
					else if(pData[0]==0x81)
					{
						TansData.B2A_81Num[0] = pData[1];
						TansData.B2A_81Num[1] = pData[2];
						TansData.B2A_81Num[2] = pData[3];
					}
					else if(pData[0]==0x82)
					{
						TansData.B2A_PlayTime[0] = pData[1];
						TansData.B2A_PlayTime[1] = pData[2];
					}
					else if(pData[0]==0x09)
					{
						TansData.B2A_PlayFoldNext = pData[1];
					}
					break;
				case 0x06:
				case 0x87:
					TansData.B2A_PlayFold = pData[1];
					break;
					
				case 0x0B:
				case 0x8B:
					TansData.B2A_NeedStartNum[0] = pData[0];
					TansData.B2A_NeedStartNum[1] = pData[1];
					TansData.B2A_NeedStartNum[2] = pData[2];
					TansData.B2A_NeedEndNum[0] = pData[3];
					TansData.B2A_NeedEndNum[1] = pData[4];
					break;
				default:break;
			}
			PostMessage(ARM_MODULE,M2A_SPDIF_INFO,SCH_WORD(pData[0],BtRx_SubID));
			break;
		case A2M_BT_IAP_CMD:
			UartTxData(SCH_Uart_BT,"AT+OTA=1\r",sizeof("AT+OTA=1\r"));
			break;
		case A2M_IAP_CMD:///===========================================================================================A2M_IAP_CMD 0x08================
			if(BtRx_SubID==0x01)
			{
				TurnOff_REM_EN;
				Set_IAP_Mode;
			}
			break;
		default:break;
	}
}
/********************************************************************************
**  Function	: M2B_TxService
**  Author		: 
**  Created on	: 20190402
**  Description	:
**  Return		: 
********************************************************************************/
void M2B_TxService(void)
{
	MESSAGE pMsg;
	SCH_U8 *pData=&BtTx_Data;
	SCH_U8 length_data = 0;
	SCH_U8 sub_id,index;
	if(FALSE==GetMessage(BT_MODULE,&pMsg))
		return;
	sub_id=LSB(pMsg.prm);
	index =MSB(pMsg.prm);
	switch(pMsg.ID)
	{
		case M2A_SYS_CMD:
			switch(sub_id)
			{
				case M2A_MCU_VER:
					length_data=sizeof(MCU_VERSION);
					sch_memcpy(pData,MCU_VERSION,length_data);
					break;
				case M2A_ARM_VER:
					length_data=TansData.ARM_Ver[0];
					sch_memcpy(pData,&TansData.ARM_Ver[1],length_data);
					break;
				default:break;
			}
			break;
		case M2B_DSP_DATA:///
			switch(sub_id)
			{
				case 0x00:
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.DspNum;
					
					//if(index == 0xff) 
					//	pData[length_data++] = 0;
					//else
					//	pData[length_data++] = App_Dsp.DspNum;
					break;
				case 0x01:///DSP状态
					pData[length_data++] = (App_Dsp.DspPwrState == DSP_NORMAL) ? 1 : 0;
					break;
				case 0x02:///混音状态
					pData[length_data++] = index;
					sch_memcpy(pData+1,App_Dsp.Dsp_Data.MixData[index],DSP_INPUT_CNT+1);
					length_data += (DSP_INPUT_CNT+1);	
					break;
				case 0x03:///General 2nd-Order Filters
					pData[length_data++] = index;
					{
						SCH_U8 Channel = index/HPLP_FILTER_CNT;
						Filter_HPLP HPLP = (Filter_HPLP)(index%HPLP_FILTER_CNT);
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Other.byte;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].MODE0;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].MODE1;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Freq>>24;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Freq>>16;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Freq>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Freq;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Gain>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Gain;
					}
					break;
				case 0x04:///EQ
					pData[length_data++] = index;
					{
						SCH_U8 Channel = index/EQ_NUM_CNT;
						EQ_NUM_T EQ_NUM = (EQ_NUM_T)(index%EQ_NUM_CNT);
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Other.byte;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].MODE0;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Boost>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Boost;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Freq>>24;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Freq>>16;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Freq>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Freq;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Gain>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Gain;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Q_Factor>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM].Q_Factor;
					}
					break;
				case 0x05:///Delay
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.Dsp_Data.DelayData[index]>>8;
					pData[length_data++] = App_Dsp.Dsp_Data.DelayData[index];
					break;
				case 0x06:///Single
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.Dsp_Data.SingleData[index]>>8;
					pData[length_data++] = App_Dsp.Dsp_Data.SingleData[index];
					if(index == DSP_CHANNEL_ALL_NONE)
					{
						pData[length_data++] = App_Dsp.Dsp_Data.SingleMaxData>>8;
						pData[length_data++] = App_Dsp.Dsp_Data.SingleMaxData;
					}
					break;
				case 0x07:///output
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.Dsp_Data.OutPutChl[index];
					break;
				case 0x08:///Mute
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.Dsp_Data.Mute[index];
					break;
				case 0x09:///ReName
					pData[length_data++] = index;
					if(index <= DSP_CHANNEL_CNT)
					{
						sch_memcpy(pData+1,App_Dsp.Dsp_Data.ReName[index],DSP_NAME_SIZE);
					}
					else if(index >= 0x81 && index <= 0x88)
					{
						index = index&0x7F;
						sch_memcpy(pData+1,App_Dsp.Dsp_ModeName.Name[index-1],DSP_NAME_SIZE);
					}
					length_data += DSP_NAME_SIZE;
					break;
				case 0x0A:
					pData[length_data++] = App_Dsp.DspUniteChannel0;
					pData[length_data++] = App_Dsp.DspUniteChannel1;
					pData[length_data++] = index;
					break;
				case 0x0B:
					pData[length_data++] = set_query_password;
					pData[length_data++] = index;
					break;
				case 0x0C:
					pData[length_data++] = index;
					if(index == 0x01)
					{
						sch_memcpy(pData+1,BT_Addr,sizeof(BT_Addr));
						length_data += sizeof(BT_Addr);
					}
					break;
				case 0x0D:///音量条
					pData[length_data++] = index;
					if(index == 0x01)
					{
						pData[length_data++] = VolData;
						pData[length_data++] = DSP_VOL_MAX;
					}
					else if(index == 0x02)
					{
						pData[length_data++] = App_Dsp.Dsp_USB_Vol;
						pData[length_data++] = App_Dsp.Dsp_USB_MAX_Vol;
					}
					break;
				case 0x0E:
					pData[length_data++] = Get_DSP_OFF_Flag ? 0x01 : 0x02;
					break;
				default:break;
			}
			break;
		case M2B_DSP_UPDATA:///
			switch(sub_id)
			{
				case 0x00:
					pData[length_data++] = index;
					if(index==0x82)///存入FLASH成功
						pData[length_data++] = App_Dsp.DspNum;
					else
						pData[length_data++] = App_Dsp.DspUpdataNum;
					break;
				case 0x01:///回应长度
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.DspUpdataLen>>24;
					pData[length_data++] = App_Dsp.DspUpdataLen>>16;
					pData[length_data++] = App_Dsp.DspUpdataLen>>8;
					pData[length_data++] = App_Dsp.DspUpdataLen;
					break;
				case 0x02:///回应数据
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.DspUpdataSn;
					{
						SCH_U8 i;
						SCH_U8 *UpData = (SCH_U8 *)(&App_Dsp.Dsp_Data);
						UpData += (64*App_Dsp.DspUpdataSn);
						///length_data += App_Dsp.DspUpdataCnt;
						///sch_memcpy(pData+2,UpData,App_Dsp.DspUpdataCnt);
						for(i=0;i<App_Dsp.DspUpdataCnt;i++)
						{
							pData[length_data++] = *UpData;
							UpData++;
						}
					}
					break;
				case 0x03:///请求长度和校验
					pData[length_data++] = index;
					break;
				case 0x04:///请求包数据
					pData[length_data++] = index;
					pData[length_data++] = App_Dsp.DspUpdataSn;
					break;
				default:break;
			}
			break;
		case M2B_SPDIF_INFO:
			switch(sub_id)
			{
				case 0x01:
					pData[length_data++] = index;
					break;
				case 0x02:
					pData[length_data++] = TansData.A2B_PlayPause_S;
					break;
				case 0x03:
					pData[length_data++] = TansData.A2B_Cycle_S;
				case 0x04:
					sch_memcpy(pData,    TansData.A2B_Curr_T, 2);
					sch_memcpy(&pData[2],TansData.A2B_Total_T,2);
					length_data += 4;
					break;
				case 0x05:
					sch_memcpy(pData,TansData.A2B_Total_T,2);
					length_data += 2;
					break;
				case 0x06:
					sch_memcpy(pData,&TansData.A2B_CatalogueName[1],TansData.A2B_CatalogueName[0]);
					length_data += TansData.A2B_CatalogueName[0];
					break;
				case 0x07:
				case 0x87:
					pData[length_data++] = TansData.A2B_TotalNum[0];
					pData[length_data++] = TansData.A2B_TotalNum[1];
					pData[length_data++] = TansData.A2B_TotalNum[2];
					break;
				case 0x08:
					sch_memcpy(pData,&TansData.A2B_ID3_Song[1],TansData.A2B_ID3_Song[0]);
					length_data += TansData.A2B_ID3_Song[0];
					break;
				case 0x09:
					sch_memcpy(pData,&TansData.A2B_ID3_Singer[1],TansData.A2B_ID3_Singer[0]);
					length_data += TansData.A2B_ID3_Singer[0];
					break;
				case 0x0A:
					sch_memcpy(pData,&TansData.A2B_ID3_Album[1],TansData.A2B_ID3_Album[0]);
					length_data += TansData.A2B_ID3_Album[0];
					break;
				case 0x0B:
				case 0x8B:
					{
						SCH_U8 SongName[64];
						if(Queue_Out(&SongName_QUEUE,SongName,1))
						{
							pData[length_data++] = SongName[0];
							pData[length_data++] = SongName[1];
							sch_memcpy(&pData[2],&SongName[3],SongName[2]);
							length_data += SongName[2];
						}
						else
						{
							return;
						}
					}
					break;
				case 0x0C:
				case 0x8C:
					pData[length_data++] = TansData.A2B_PlayNum[0];
					pData[length_data++] = TansData.A2B_PlayNum[1];
					break;
				default:break;
			}
			break;
		default:break;
	}
	BtTx_Head1 = HEAD_ADDRESS_MCU;
	BtTx_Head2 = HEAD_ADDRESS_BT;
	BtTx_Length = length_data+7;
	BtTx_Sequence++;
	BtTx_GroupID = pMsg.ID;
	BtTx_SubID = sub_id;
	BtTxModuel.TxData_Parity = GetCheckData_Xor(BtTxModuel.TxData,BtTx_Length-1);
	BtTxModuel.TxData[BtTx_Length-1] = BtTxModuel.TxData_Parity;
	UartTxData(Uart_CONNECT, BtTxModuel.TxData, BtTx_Length);
	BtTxModuel.Check_Ack=1;
	BtTxModuel.Check_ResendCounte=0;
	BtTxModuel.Check_ResendTimer=0;
	BtTxModuel.Check_Busy=1;
}
/********************************************************************************
**  Function	: TASK_Bt_Pro
**  Author		: 
**  Created on	: 20190402
**  Description	:
**  Return		: 
********************************************************************************/
void TASK_Bt_Pro(void)
{
	if(/*Get_OsStartOk==OFF||*/Get_SysPower_Flag==OFF)  
	{
		return;
	}
	if(BtTxModuel.Check_Ack)
	{
		BtTxModuel.Check_ResendTimer++;
		if(((BtTxModuel.Check_ResendTimer==T200MS_8)&&(Uart_CONNECT == SCH_Uart_PC))
			||((BtTxModuel.Check_ResendTimer==T200MS_8)&&(Uart_CONNECT == SCH_Uart_PC1))
			||((BtTxModuel.Check_ResendTimer==T480MS_8)&&(Uart_CONNECT == SCH_Uart_BT)))
		{  	
			BtTxModuel.Check_ResendTimer=0;
			if(++BtTxModuel.Check_ResendCounte<3)
			{
				 UartTxData(Uart_CONNECT, BtTxModuel.TxData, BtTx_Length);
			}
			else
			{
				BtTxModuel.Check_ResendCounte=0;
				BtTxModuel.Check_Busy=0;
				BtTxModuel.Check_Ack=0;
			}
		}
	}
	else if(!BtTxModuel.Check_Busy)
	{
		M2B_TxService();
	}
}

