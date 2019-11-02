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
			break;
		default:break;
	}
}
///===============================================



