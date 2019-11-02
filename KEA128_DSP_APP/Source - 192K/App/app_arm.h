///=========================
#ifndef __APP_ARM_H
#define __APP_ARM_H

#define Uart_ARM            SCH_Uart1


#define COMM_NACK_NG              0xF0
#define COMM_NACK_NO_SUPPORT      0xF3
#define COMM_NACK_BUSY            0xFC
#define COMM_ACK                  0xFF

typedef enum
{
	A2M_IDLE_CMD=0x00,
	A2M_SYS_CMD=0x01,
	A2M_IAP_CMD=0x08,
	A2M_SPDIF_INFO = 0x0C,
	A2M_DSP_DATA_STORE = 0x0E,
	A2M_ACK = 0xFF
}A2M_GROUP_ID;

#define M2A_KEY_SUB	     0x01
typedef enum
{
	M2A_SYS_CMD=0x01,
	M2A_IAP_CMD=0x08,
	M2A_SPDIF_INFO = 0x0C,
	M2A_DSP_DATA_STORE = 0x0E,
    M2A_END=0xFF
}M2A_GROUP_ID;


typedef enum
{
	M2A_REQUEST=0x01,
	M2A_POWER,
	M2A_MCU_VER,
	M2A_CAN_VER,
	M2A_VOLUME,
	M2A_PARK,
	M2A_REVERSE,
	M2A_VOLTAGE,
	M2A_SETUP,
	M2A_SOURCE,
	M2A_SCREEN,
	M2A_WORK_STATE,
	M2A_ARM_VER=0x11,
}M2A_SYS_SUBID;
typedef enum
{
	A2M_SYS_OK=0x01,
	A2M_SETUP,
	A2M_SETTING,
	A2M_MCU_VER,
	A2M_CAN_VER,
	A2M_PANEL_TYPE,
	A2M_SOURCE,
	A2M_BT_STATE,
	A2M_G3_STATE,
	A2M_WORK_STATE,
	A2M_ARM_VER=0x11,
}A2M_SYS_SUBID;

typedef struct 
{
	SCH_U8 ARM_Ver[32];        ///ARM版本号
	SCH_U8 B2A_PlayNum[2];     ///当前播放歌曲编号
	SCH_U8 B2A_81Num[2];       ///播放时间 0x81
	SCH_U8 B2A_PlayTime[2];    ///播放时间 0x82
	SCH_U8 B2A_SongCnt[2];     ///总歌曲数
	SCH_U8 B2A_NeedStartNum[2];///请求目录名起始编号
	SCH_U8 B2A_NeedEndNum[2];  ///请求目录名终止编号
	SCH_U8 A2B_PlayNum[2];     ///当前目录当前播放编号
	SCH_U8 A2B_TotalNum[2];    ///当前目录编号总数
	SCH_U8 A2B_PlayPause_S;    ///播放暂停状态
	SCH_U8 A2B_Cycle_S;        ///循环状态
	SCH_U8 A2B_Curr_T[2];      ///当前时间
	SCH_U8 A2B_Total_T[2];     ///总时间
	SCH_U8 A2B_ID3_Song[50];   ///歌曲名
	SCH_U8 A2B_ID3_Singer[50]; ///歌手
	SCH_U8 A2B_ID3_Album[50];  ///专辑名
	SCH_U8 A2B_CatalogueName[50];   ///
	SCH_U8 A2B_SongName[15][64];   ///编号下的名称
}Tanspond_A_B_Data;
extern Tanspond_A_B_Data TansData;
extern QUEUE_T SongName_QUEUE;

extern void ArmDataRxPro(void);
extern void TASK_Arm_Pro(void);

#endif
