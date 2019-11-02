///=========================
#ifndef __APP_VOLTAGE_H
#define __APP_VOLTAGE_H

#define	N_8V	0
#define	N_9V	1
#define	N_16V	2
#define	N_17V	3

#define	PWRON_VOLT_8V       0x666
#define	PWRON_VOLT_9V       0x70E
#define	PWRON_VOLT_16V      0xBC0
#define	PWRON_VOLT_17V      0xC68

#define	PWROFF_VOLT_8V      0x603
#define	PWROFF_VOLT_9V      0x6B0
#define	PWROFF_VOLT_16V     0xB55
#define	PWROFF_VOLT_17V     0xC02



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
