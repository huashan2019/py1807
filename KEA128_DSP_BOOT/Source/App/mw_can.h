///=========================
#ifndef __MW_CAN_H
#define __MW_CAN_H



#define CAN0_FUNC    ENABLE

#define IO_CAN0_STD                GPIOB
#define GPIO_CAN0_STD              GPIO_PTH5//////
#define TurnOn_CAN0_STD        	   GPIO_PinSet(GPIO_CAN0_STD)
#define TurnOff_CAN0_STD     	   GPIO_PinClear(GPIO_CAN0_STD)
#define IO_CAN0_ERR                GPIOB
#define GPIO_CAN0_ERR              GPIO_PTH6//////
#define CAN0_ERR_LEVEL             GPIO_ReadInputPin(GPIO_CAN0_ERR)
#define CAN0_ERR_LVON              (LOW_LEVEL==GPIO_CAN0_ERR)
#define CAN0_ERR_LVOFF             (HIGH_LEVEL==GPIO_CAN0_ERR)

///=================================================
typedef struct
{
    SCH_U32 u32IDAR0;
    SCH_U32 u32IDAR1;
	SCH_U32 u32IDMR0;
	SCH_U32 u32IDMR1;
}Can_Filter_C;
///=================================================

typedef enum
{
	Can_Rx,
	Can_Tx
}Can_RT;

typedef enum
{
	SCH_Can0,
	SCH_Can_Num
}Can_T;

typedef enum
{
    FrameType_Std = 0, /* Standard frame */
	FrameType_Ext /* Extend frame */
}CAN_FrameType;  /* MSCAN frame type */

typedef struct  
{
	SCH_U32 Id;///ID
	SCH_U8 IDE;///   0---STD   1---EXT
	SCH_U8 RTR;///   0---DATA  1---REMOTE
	SCH_U8 DLC;///length 0---8
	SCH_U8 Data[8];
}Can_Data_T;

extern SCH_U16 CanTxCnt(Can_T can);
extern SCH_U16 CanRxCnt(Can_T can);
extern SCH_BOOL CanTxData(Can_T can, Can_Data_T *const data, SCH_U16 Len);
extern SCH_BOOL CanRxData(Can_T can, Can_Data_T *data, SCH_U16 Len);
extern void Can_Tx_DataPro(Can_T can);
extern void Can_Rx_DataPro(Can_T can,Can_Data_T *const data);
extern void SysCanExit(Can_T can);
extern SCH_BOOL SysCanInit(Can_T can);
#endif
