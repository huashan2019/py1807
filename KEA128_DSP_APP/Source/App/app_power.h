///=========================
#ifndef __APP_POWER_H
#define __APP_POWER_H







typedef enum
{
	NO_EVT,
	EVT_POWER_ON,
	EVT_POWER_OFF
}POWER_EVT;

typedef enum 
{
	POWER_STANDBY,
	POWER_ON,
	POWEROFF_FROM_MANNED,
	POWEROFF_FROM_VOLTAGE,
	POWEROFF_FROM_START
}POWEROFFREASON;


typedef enum
{
    POWER_ON_START,    
    POWER_ON_DELAY,
    POWER_ARM_RESET,
    POWER_VAR_RECOVER,
    POWER_TFT_POWER_ON,
    POWER_SECURITY_CODE,
    POWER_NORMAL_RUN,

    POWER_CLOSE_SCREEN,
    POWER_OFF_DELAY,
    POWER_SYSTEM_STANDBY,
    POWER_ACCOFF,
    POWER_ACCOFF2,
    POWER_ARM_ERR_RESET
}Sys_POWER_STATE;


typedef struct 
{
	Sys_POWER_STATE nPowerState;
	POWEROFFREASON bk_PowerOffReason;
	POWEROFFREASON PowerOffReason;
	SCH_U16 Power_Timer;
	SCH_U16 nWaitArmHandShake;
}Sys_Power_T;
extern Sys_Power_T SysPower;



extern void TASK_Power_Pro(void);

#endif
