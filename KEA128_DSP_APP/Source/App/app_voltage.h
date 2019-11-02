///=========================
#ifndef __APP_VOLTAGE_H
#define __APP_VOLTAGE_H

#define	N_8V	0
#define	N_9V	1
#define	N_16V	2
#define	N_17V	3

#define	PWRON_VOLT_8V       0x618///0x5E0///
#define	PWRON_VOLT_9V       0x6E0///0x69D///
#define	PWRON_VOLT_16V      0xCA0///0xBC1///
#define	PWRON_VOLT_17V      0xD50///0xC7D///
///20180530  高电压开关机修改成 17V  18V
#define	PWROFF_VOLT_8V      0x618///0x618实际值8///0x5E0理论值8///---
#define	PWROFF_VOLT_9V      0x6E0///0x6E0实际值9///0x69D理论值9///
#define	PWROFF_VOLT_16V     0xCA0///0xBF0实际值16///0xBC1理论值16///
#define	PWROFF_VOLT_17V     0xD50///0xCA0实际值17///0xC7D理论值17///0xD50实际值18///0xD39理论值18///



typedef enum
{
	LOW_ERROR,	
	V_NORMAL,	
	HIGHT_ERROR	
}VOLTAGE_STATE;

typedef struct 
{
	SCH_U16 ADC_current;
	SCH_U8 BUTimerOut;
	VOLTAGE_STATE VoltageState;
}Vol_Det_T;
extern Vol_Det_T VolDet;

#define BATTERY_VOLT_DET_AD     	ADC_CHANNEL_AD11

extern void TASK_Voltage_Det(void);

#endif
