/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_can.c  ---   CAN
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

SCH_U8 CAN_DataRx[13];
SCH_U8 CAN_DataTx[13];

void CanDataRxPro(void)
{
	Can_Data_T Can_Data;
	while(CanRxCnt(SCH_Can0))
	{
		CanRxData(SCH_Can0, &Can_Data, 1);
		if(Can_Data.IDE==0)
		{
			switch(Can_Data.Id)
			{
				case 0x0330:
					CanTxData(SCH_Can0,&Can_Data,1);
					break;
				default:break;
			}
		}
		if(Can_Data.IDE==1)
		{
			switch(Can_Data.Id)
			{
				case 0x00001314:
					PostMessage(CAN_MODULE,M2C_TEST,NONE);
					break;
				case 0x18D7E7E8:
					CAN_DataRx[0] = Can_Data.Id>>0;
					CAN_DataRx[1] = Can_Data.Id>>8;
					CAN_DataRx[2] = Can_Data.Id>>16;
					CAN_DataRx[3] = Can_Data.Id>>24;
					CAN_DataRx[4] = Can_Data.IDE;
					sch_memcpy(&CAN_DataRx[5],Can_Data.Data,8);
					//InsertMessage(ARM_MODULE,M2A_CAN_CMD,M2A_CAN_DATA);
					PostMessage(ARM_MODULE,M2A_CAN_CMD,M2A_CAN_DATA);
					break;
				default:break;
			}
		}
	}
}

void M2C_TxService(void)
{
	MESSAGE pMsg;
	Can_Data_T Can_Data;
	if(FALSE==GetMessage(CAN_MODULE,&pMsg))
		return;
	switch(pMsg.ID)
	{
		case M2C_TEST:////测试使用  无意义
			Can_Data.Id = 0x00001413;
			Can_Data.IDE = FrameType_Ext;
			Can_Data.RTR = 0;
			Can_Data.DLC = 8;
			CanTxData(SCH_Can0,&Can_Data,1);
			break;
		case M2C_DATA:
			Can_Data.Id = CAN_DataTx[3]<<24|CAN_DataTx[2]<<16|CAN_DataTx[1]<<8|CAN_DataTx[0];
			Can_Data.IDE = CAN_DataTx[4];
			Can_Data.RTR = 0;
			Can_Data.DLC = 8;
			sch_memcpy(Can_Data.Data,&CAN_DataTx[5],8);
			CanTxData(SCH_Can0,&Can_Data,1);
			break;
		default:break;
	}
}

void TASK_Can_Pro(void)
{
	M2C_TxService();
}




