///=========================
#ifndef __APP_ARM_H
#define __APP_ARM_H

#define SCH_Uart_BT   SCH_Uart0  ///---  BT
#define SCH_Uart_PC   SCH_Uart1  ///---  PC   
extern Uart_T Uart_ARM;

#define COMM_NACK_NG              0xF0
#define COMM_NACK_NO_SUPPORT      0xF3
#define COMM_NACK_BUSY            0xFC
#define COMM_ACK                  0xFF

typedef enum
{
	A2M_IDLE_CMD=0x00,
	A2M_SYS_CMD=0x01,
	A2M_UICC_CMD,
	A2M_RADIO_CMD,
	A2M_CAN_CMD,
	A2M_STUDY_KEY_CMD,
	A2M_AUDIO_CMD,
	A2M_USART_CMD,
	A2M_IAP_CMD,
	A2M_AUTO_TEST_CMD,
	A2M_ACK = 0xff
}A2M_GROUP_ID;

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
	A2M_WORK_STATE
}A2M_SYS_SUBID;


#define M2A_KEY_SUB	     0x01
typedef enum
{
	M2A_SYS_CMD=0x01,
	M2A_UICC_CMD,
	M2A_RADIO_CMD,
	M2A_CAN_CMD,
	M2A_STUDY_KEY_CMD,
	M2A_AUDIO_CMD,
	M2A_USART_CMD,
	M2A_IAP_CMD,
	M2A_AUTO_TEST_CMD,
    M2A_END=0xff
}M2A_GROUP_ID;

typedef enum
{
	M2A_CAN_DATA=0x02,
	M2A_CAN_AAC=0x03,
	M2A_CAN_LIN=0x04,
	M2A_CAN_RADAR=0x05
}M2A_CAN_SUBID;


typedef enum
{
	RADIO_REQUEST=0x01,
	RADIO_INFO=0x02,
	RADIO_FREQLIST=0x03,
	RADIO_PTY=0x04,
	RADIO_PS=0x05,
	RADIO_TEXT=0x06
}M2A_RADIO_SUBID;

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
	M2A_WORK_STATE
}M2A_SYS_SUBID;

typedef enum
{
	ASP_REQUEST=0x01,
	ASP_SETUP=0x02,
	ASP_EQ=0x03,
	ASP_BAND=0x04
}M2A_EQ_SUBID;





typedef enum
{
	A2M_IAP_REQ=0x01,       ///请求升级
	A2M_IAP_DATA=0x02,      ///发送的数据
	A2M_IAP_C_L=0x03,       ///发送的
	A2M_IAP_RESET=0x04,     ///允许复位
}A2M_IAP_D;


typedef enum
{
	M2A_IAP_REQ_C_L=0x01,    ///请求长度校验
	M2A_IAP_REQ_DATA=0x02,   ///请求数据
	M2A_IAP_REQ_RESET=0x03,  ///请求复位
}M2A_IAP_D;

extern void ArmDataRxPro(void);
extern void TASK_Arm_Pro(void);

#endif
