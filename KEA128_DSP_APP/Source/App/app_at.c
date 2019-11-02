/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_at.c  ---   AT指令
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
SCH_U8 BT_Addr[12];
void AtDataAnalyse(SCH_U8 *Data)
{
	static SCH_U8 index;
	if(sch_compare(Data,"\r\n",2)==FALSE)
		return;
	switch(Data[2])
	{
		case 'I':
			if(sch_compare(&Data[3],"M_READY",7)==TRUE)
			{
				SysWaitMs(1);
				UartTxData(SCH_Uart_BT,BT_NAME_GET,sizeof(BT_NAME_GET));
			}
			else if(sch_compare(&Data[3],"M_GATT",6)==TRUE)///蓝牙连接上了
			{
				
			}
			break;
		case 'O':
			break;
		case 'E':
			break;
		case '+':
			if(sch_compare(&Data[3],"LBDADDR:",8)==TRUE)
			{
				sch_memcpy(BT_Addr,&Data[11],sizeof(BT_Addr));
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0C));
			}
			else if(sch_compare(&Data[3],"NAME:",5)==TRUE)
			{
				if(sch_compare(&Data[8],"DSP_DSD",7)==FALSE)
				{
					SysWaitMs(1);
					UartTxData(SCH_Uart_BT,BT_NAME_SET,sizeof(BT_NAME_SET));
				}
				else if(sch_compare(&Data[13],"_EVA",4)==TRUE)///蓝牙出错了->复位重启
				{
					SysWaitMs(1);
					UartTxData(SCH_Uart_BT,BT_FACTORYRESET,sizeof(BT_FACTORYRESET));
					FeedDog();
					while(1);
				}
				else
				{
					SysWaitMs(1);
					UartTxData(SCH_Uart_BT,BT_LBDADDR_GET,sizeof(BT_LBDADDR_GET));
				}
			}
			else if(sch_compare(&Data[3],"SCAN=",5)==TRUE)
			{
				if(Data[41] == index)
					break;
				index = Data[41];
				if(Data[43] == 0x31)
				{
					SCH_U8 Mute = (App_Dsp.Dsp_Data.Mute[DSP_CHANNEL_ALL_NONE] == DSP_UNMUTE) ? DSP_MUTE : DSP_UNMUTE;
					Dsp_Mute(DSP_CHANNEL_ALL_NONE,Mute);
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x08));
				}
				else if(Data[43] == 0x32)
				{
					VolData = GetMin(DSP_VOL_MAX,VolData + 1);
					Vol_Ctl(VolData);
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0D));
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x06));
				}
				else if(Data[43] == 0x33)
				{
					VolData = VolData ? VolData - 1 : 0;
					Vol_Ctl(VolData);
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0D));
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(DSP_CHANNEL_ALL_NONE,0x06));
				}
			}
			break;
		default:break;
	}
}
///===============================================



