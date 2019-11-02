///=========================
#ifndef __APP_CAN_H
#define __APP_CAN_H

typedef enum
{
	M2C_IDLE,		
	M2C_TEST,
	M2C_AAC,
	M2C_DATA
}M2C_ID;

extern SCH_U8 CAN_DataRx[13];
extern SCH_U8 CAN_DataTx[13];



extern void CanDataRxPro(void);
extern void TASK_Can_Pro(void);
#endif
