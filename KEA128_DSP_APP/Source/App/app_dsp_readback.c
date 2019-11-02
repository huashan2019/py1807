/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_readback.c  ---   与dsp 检测
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.04
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

///==================================================================================readback========
const SCH_U16 Mix_READBACK_addr[DSP_CHANNEL_CNT]=
{
	MOD_READBACK1_READBACKALGNEWSIGMA3001VALUE_ADDR,
	MOD_READBACK1_2_READBACKALGNEWSIGMA3002VALUE_ADDR,
	MOD_READBACK1_3_READBACKALGNEWSIGMA3003VALUE_ADDR,
	MOD_READBACK1_4_READBACKALGNEWSIGMA3004VALUE_ADDR,
	MOD_READBACK1_5_READBACKALGNEWSIGMA3005VALUE_ADDR,
	MOD_READBACK1_6_READBACKALGNEWSIGMA3006VALUE_ADDR,
	MOD_READBACK1_7_READBACKALGNEWSIGMA3007VALUE_ADDR,
	MOD_READBACK1_8_READBACKALGNEWSIGMA3008VALUE_ADDR
};
SCH_U8 Dsp_Audio_Flag[DSP_CHANNEL_CNT];
SCH_U8 Dsp_REM_Flag[DSP_CHANNEL_CNT]={1,1,1,1,1,1,1,1};///REM关闭标志
double Data_dbe[8];
const double Data_dbe_MaxMin[8][3]=
{
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
	{0.00130,  0.00200,  0.00500},
};
void Dsp_Info_Data(SCH_U8 Channel,SCH_U8 *Cnt)
{
	SCH_U8 DataBuff[4];
	SCH_S32 Data_S32;
	SIGMA_READ(DEVICE_ADDR_IC_1|0x01,Mix_READBACK_addr[Channel-1],DataBuff);
	Data_S32 = (DataBuff[0]<<24)+(DataBuff[1]<<16)+(DataBuff[2]<<8)+DataBuff[3];
	Data_dbe[Channel-1] = fabs((double)Data_S32/0x1000000);
	if(Data_dbe[Channel-1] == 0)
	{
		Dsp_REM_Flag[Channel-1] = 0;
	}
	else if(Data_dbe[Channel-1] < Data_dbe_MaxMin[Channel-1][0])
	{
		if(*Cnt >= 50)
		{
			Dsp_REM_Flag[Channel-1] = 0;
		}
		if(++*Cnt >= 100)
		{
			*Cnt = 0;
			if(Dsp_Audio_Flag[Channel-1] == 0)
			{
				Dsp_Audio_Flag[Channel-1] = 1;
				Dsp_Mute_A(Channel,DSP_MUTE,1);
				///TurnOff_REM_EN;
			}
		}
		if(Dsp_Audio_Flag[Channel-1] == 1)
			*Cnt = 0;
	}
	else if(Data_dbe[Channel-1] > Data_dbe_MaxMin[Channel-1][1])
	{
		if(Data_dbe[Channel-1] > Data_dbe_MaxMin[Channel-1][2])
		{
			if(SysPower.nPowerState==POWER_NORMAL_RUN 
				&& !SysPower.Power_First_Timer
				&& (Get_ACC_Has && Get_ACC_Flag || !Get_ACC_Has && Get_AUDIO_Flag))
			{
				if(++*Cnt >= 10)
				{
					*Cnt = 0;
					Dsp_REM_Flag[Channel-1] = 1;
					TurnOn_REM_EN;
					
					Printf("REM ON : %x \n", SysPower.nPowerState);
				}
			}
		}
		else
		{
			*Cnt = 0;
		}
		if(Dsp_Audio_Flag[Channel-1] == 1)
		{
			Dsp_Audio_Flag[Channel-1] = 0;
			if(App_Dsp.Dsp_Data.Mute[Channel] == DSP_UNMUTE)
			{
				Dsp_Mute_A(Channel,DSP_UNMUTE,1);
			}
		}
	}
	else
	{
		*Cnt = 0;
	}
}
void Dsp_Info_Det(void)
{
	SCH_U8 index;
	static SCH_U8 Timer=0;
	static SCH_U8 Cnt[DSP_CHANNEL_CNT];
	if(App_Dsp.DspPwrState != DSP_NORMAL)
		return;
	if(SysPower.nPowerState != POWER_NORMAL_RUN)
		return;
	if(App_Dsp.DSP_Updata_State != UpData_Idle)
		return;
	switch(Timer)
	{
		case 0:
			Dsp_Info_Data(1,&Cnt[0]);
			break;
		case 1:
			Dsp_Info_Data(2,&Cnt[1]);
			break;
		case 2:
			Dsp_Info_Data(3,&Cnt[2]);
			break;
		case 3:
			Dsp_Info_Data(4,&Cnt[3]);
			break;
		case 4:
			Dsp_Info_Data(5,&Cnt[4]);
			break;
		case 5:
			Dsp_Info_Data(6,&Cnt[5]);
			break;
		case 6:
			Dsp_Info_Data(7,&Cnt[6]);
			break;
		case 7:
			Dsp_Info_Data(8,&Cnt[7]);
			break;
		default:break;
	}
	if(Timer++ >= T96MS_8)
	{
		Timer = 0;
	}
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		if(Dsp_REM_Flag[index] == 1)
			break;
	}
	if(index == DSP_CHANNEL_CNT)
	{
		TurnOff_REM_EN;
		
		Printf("REM OFF2 : %x \n", SysPower.nPowerState);
	}
}
///===========================================================================================================





