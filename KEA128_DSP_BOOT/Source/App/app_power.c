/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_power.c  ---   系统开关机管理
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Sys_Power_T SysPower;

void SystemPowerUp(void)
{
	ClearAllModeMessage();
	GPIOInit();
	AudioMute(HARDON);
	TurnOn_ARM_PWR;
	TurnOn_CCFL_POWER;
	ARM_RESET_HOLD;
	SYS_Power_Ctl(ON);
	CCFL_Power_Ctl(OFF);
	PostMessage(TUNER_MODULE, EVT_TUN_ON, NONE);
	ASP_POWER_ON();	
	///ARM_RESET_RELEASE;
	///SYS_3V3_ON;
	SYS_12V_ON;
	USB1_EN_ON;
	USB_EN_ON;;
	GPS_POWER_ON;
}

void PowerOnSystemModule(void)
{
	Amp_Init();
	Bsp_TIM_Init();
	Bsp_UART_Init();
	Bsp_CAN_Init();
	Bsp_ADC_Init();
	I2C0Init();
	ARM_RESET_RELEASE;
}
void PowerOffSystemModule(void)
{
	CCFL_Power_Ctl(OFF);
	AudioMute(SOFTON);
	AudioMute(HARDON);
	PostMessage(TUNER_MODULE,EVT_TUN_SEEK_BREAK, NONE);
}

void EnterPowerOff(void)
{
	TurnOff_AMP;
	AudioMute(HARDON);
	TurnOff_ARM_PWR;
	TurnOff_CCFL_POWER;
	SYS_Power_Ctl(OFF);
	ARM_RESET_HOLD;
	///SYS_3V3_OFF;
	SYS_12V_OFF;
	USB1_EN_OFF;
	USB_EN_OFF;
	GPS_POWER_OFF;
	ARM_REV_OFF;
	RadioPowerOff();
	ASP_POWER_OFF();
	Clr_OsStartOk;
	Clr_AppStartOk;
}

void PowerMessage(void)
{
	MESSAGE pMsg;
	if(FALSE==GetMessage(POWER_MODULE,&pMsg))
		return;
	switch(pMsg.ID)
	{
		case EVT_POWER_ON:
			SysPower.bk_PowerOffReason=SysPower.PowerOffReason;
			SysPower.PowerOffReason=POWER_ON;
			SysPower.nPowerState=POWER_ON_DELAY;
			SysPower.Power_Timer=0;
			AudioMute(HARDON);
			CCFL_Power_Ctl(OFF);
			ClearAllModeMessage();
			break;
		case EVT_POWER_OFF:
			SysPower.PowerOffReason = (POWEROFFREASON)pMsg.prm;
			if(SysPower.PowerOffReason==POWEROFF_FROM_ACC)
			{
				if(SysPower.nPowerState>=POWER_CLOSE_SCREEN)
					break;
				SysPower.nPowerState=POWER_ACCOFF;
				SysPower.Power_Timer = 0;
			}
			else if(SysPower.PowerOffReason==POWEROFF_FROM_VOLTAGE)
			{
				if(SysPower.nPowerState>=POWER_CLOSE_SCREEN)
					break;
				SysPower.nPowerState=POWER_CLOSE_SCREEN;
				SysPower.Power_Timer = 0;
			}
			PowerOffSystemModule();
			PostMessage(ARM_MODULE, M2A_SYS_CMD, M2A_POWER);
			break;
		default:break;
	}
}

void TASK_Power_Pro(void)
{
	PowerMessage();
	SysPower.Power_Timer++;
	switch(SysPower.nPowerState)
	{
		case POWER_ON_START:
			if(SysPower.Power_Timer>=T40MS_8)
			{
				if(Get_ACC_Flag&&!Get_VolErr_Flag)
				{
					PostMessage(POWER_MODULE,EVT_POWER_ON,0);  
				}
				else
				{
					if(Get_VolErr_Flag)
						SysPower.PowerOffReason=POWEROFF_FROM_VOLTAGE;
					SysPower.nPowerState=POWER_OFF_DELAY;
					SysPower.Power_Timer=0;  
				}
			}
			break;
		case POWER_ON_DELAY:
			SysPower.nPowerState=POWER_VAR_RECOVER;
			SysPower.Power_Timer=0;             			
			SystemPowerUp();
			break;
		case POWER_VAR_RECOVER:
			if(SysPower.Power_Timer>=T480MS_8)
			{
				SysPower.Power_Timer=0;
				SysPower.nPowerState=POWER_TFT_POWER_ON;			       
				PowerOnSystemModule();
				PostMessage(MAIN_MODULE,EVT_SRC_POWER_ON,T25S_16);
			}
			break;
		case POWER_TFT_POWER_ON:
			if(SysPower.Power_Timer>=T96MS_8)
			{
				SysPower.Power_Timer=0;
				SysPower.PowerOffReason=POWER_ON;/**/
				SysPower.nPowerState=POWER_SECURITY_CODE;
				rTw8816CtrlSubSt = 5;
			}
			break;
		case POWER_SECURITY_CODE:
			if(SysPower.Power_Timer>=T4S_8||Get_OsStartOk&&SysPower.Power_Timer>=T320MS_8)
			{
				Get_OsStartOk=1;///
				PostMessage(MMI_MODULE,UICC_TFT_AWAKE,T16MS_8);
				PostMessage(MAIN_MODULE,EVT_SRC_POWER_ON,T_NOW);
				SysPower.nPowerState=POWER_NORMAL_RUN;
				Printf("PowerState = POWER_NORMAL_RUN \n");
			}
			break;
		case POWER_NORMAL_RUN:
			if(Get_AppStartOk==0&&Get_REV_Flag==0)
			{
				SysPower.nWaitArmHandShake++;
				if(SysPower.nWaitArmHandShake>=T15S_8)
					Set_AppStartOk;
			}
			else
			{
				SysPower.nWaitArmHandShake=0;
			}
			break;
		case POWER_CLOSE_SCREEN:
			if(SysPower.Power_Timer>=T24MS_8)
			{
				EnterPowerOff();
				SysPower.Power_Timer=0;
				SysPower.nPowerState=POWER_OFF_DELAY;
			}
			break;
		case POWER_OFF_DELAY:
			if(Get_ACC_Flag)
				SysPower.nPowerState=POWER_SYSTEM_STANDBY;
			else
				SysPower.nPowerState=POWER_ACCOFF;
			break;
		case POWER_SYSTEM_STANDBY:
			if(Get_ACC_Flag&&!Get_VolErr_Flag)
			{
				PostMessage(POWER_MODULE,EVT_POWER_ON,0);  
			}
			break;
		case POWER_ACCOFF:
			if(((SysPower.Power_Timer >= T6S_8)&&(Get_OsStartOk))
				||((SysPower.Power_Timer >= T240MS_8)&&(!Get_OsStartOk)))
			{            
				EnterPowerOff();
				SysPower.nPowerState=POWER_ACCOFF2;
				SysPower.Power_Timer=0;
			}   
			else if(Get_ACC_Flag&&Get_OsStartOk)////
			{
				SysPower.nPowerState=POWER_TFT_POWER_ON;
			}
			break;
		case POWER_ACCOFF2:
			if(SysPower.Power_Timer >= T240MS_8)
			{
				if(!Get_ACC_Flag)
				{
					Set_SLEEP_Mode;
				}			       
				SysPower.nPowerState=POWER_SYSTEM_STANDBY;
				Printf("PowerState = POWER_SYSTEM_STANDBY \n");
			}
			break;
		default:break;
	}
}

