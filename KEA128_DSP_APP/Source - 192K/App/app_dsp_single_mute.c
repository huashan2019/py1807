/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_single_mute.c  ---   ”Îdsp “Ù¡ø æ≤“Ù
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.04
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

SCH_U8 VolData = DSP_VOL_DEFAULT;


void Dsp_Single_Mute_Init(void)
{
	SCH_U16 index;	
	App_Dsp.Dsp_Data.SingleData[DSP_CHANNEL_ALL_NONE] = DSP_SINGLE_ALL_DEFAULT;
	for(index=1;index<(DSP_CHANNEL_CNT+1);index++)
	{
		App_Dsp.Dsp_Data.SingleData[index] = DSP_SINGLE_DEFAULT;
	}
	App_Dsp.Dsp_Data.SingleMaxData = DSP_SINGLE_ALL_MAX;
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		App_Dsp.Dsp_Data.Mute[index] = DSP_UNMUTE;///unmute
	}
}
///================================================================================================Single========
const SCH_U16 Single_addr[9]=
{
	MOD_SINGLE2_ALG0_TARGET_ADDR,
	MOD_DELAY_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_2_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_3_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_4_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_5_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_6_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_7_SINGLE1_ALG0_TARGET_ADDR,
	MOD_DELAY_8_SINGLE1_ALG0_TARGET_ADDR
};
void Dsp_Single_A(SCH_U8 Channel,SCH_U16 data)
{
	double Data,index;
	SCH_U8 buff[4] = {0x00, 0x00, 0x20, 0x8A};
	data = LimitMaxMin(DSP_SINGLE_MIN, data, DSP_SINGLE_MAX);
	Data = ((double)data-8000)/100;
	index = pow(10,Data/20);
	SIGMA_WRITE_REGISTER_BLOCK(    DEVICE_ADDR_IC_1, Single_addr[Channel]+1,  4, buff);
	SIGMASTUDIOTYPE(index,buff);
	SIGMA_SAFELOAD_WRITE_REGISTER( DEVICE_ADDR_IC_1, Single_addr[Channel],    1, buff);
	SysWaitMs(1);
}
void Dsp_Single(SCH_U8 Channel,SCH_U16 data)
{
	SCH_U16 startVol,targetVol;
	if(Channel > DSP_CHANNEL_CNT)
		return;
	if(Channel == DSP_CHANNEL_ALL_NONE)
		data = LimitMaxMin(DSP_SINGLE_MIN, data, App_Dsp.Dsp_Data.SingleMaxData);
	startVol = App_Dsp.Dsp_Data.SingleData[Channel]/100;
	targetVol = data/100;
	while(1)
	{	        
		if(startVol>targetVol)
			--startVol;       
		else if(startVol<targetVol)
			++startVol;
		Dsp_Single_A(Channel,startVol*100);
		if(startVol==targetVol)
		{
			Dsp_Single_A(Channel,data);
			break;
		}
	}
	App_Dsp.Dsp_Data.SingleData[Channel] = data;
}
void Dsp_VolUpdate(SCH_U8 Channel,MUTE_T MUTE_Type)
{
	SCH_U16 startVol,targetVol;
	if(MUTE_Type == SOFTON)
	{
		startVol = App_Dsp.Dsp_Data.SingleData[Channel]/100;
		targetVol = 0x0000;
	}
	else if(MUTE_Type == SOFTOFF)
	{
		startVol = 0x0000;
		targetVol = App_Dsp.Dsp_Data.SingleData[Channel]/100;
	}
	while(1)
	{	        
		if(startVol>targetVol)
			--startVol;       
		else if(startVol<targetVol)
			++startVol;
		Dsp_Single_A(Channel,startVol*100);
		if(startVol==targetVol)
		{
			if(MUTE_Type == SOFTOFF)
				Dsp_Single_A(Channel,App_Dsp.Dsp_Data.SingleData[Channel]);
			break;
		}
	}
}
const SCH_U16 VolList[DSP_VOL_MAX+1]={
	   0, 
   /*200,  400,  600,  800, 2000, 1200, 1400, 1600, 1800, 2000,
	2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000,
	4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000,
	6200, 6400, 6600, 6800, 7000, 7200, 7400, 7600, 7800, 8000,*/
  /*4100, 5000, 5320, 5650, 5840, 6020, 6160, 6280, 6400, 6490,
	6580, 6650, 6720, 6790, 6850, 6910, 6960, 7010, 7050, 7110,
	7150, 7190, 7230, 7260, 7300, 7330, 7370, 7400, 7430, 7460,
	7490, 7520, 7550, 7580, 7600, 7610, 7630, 7660, 7690, 8000,*/
	 100,  900, 1700, 2300, 2700, 3000, 3300, 3500, 3700, 3800,
	3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800,
	4900, 5000, 5200, 5400, 5600, 5800, 6000, 6200, 6400, 6600,
	6800, 7000, 7200, 7400, 7500, 7600, 7700, 7800, 7900, 8000,
};
void CheckVol(void)
{
	SCH_U8 index;
	for(index=0; index <= DSP_VOL_MAX; index++)
	{
		if(App_Dsp.Dsp_Data.SingleData[DSP_CHANNEL_ALL_NONE] < VolList[index])
		{
			VolData = index;
			PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x01,0x0D));
			return;
		}
	}
}
void Vol_Ctl(SCH_U8 Data)
{
	VolData = GetMin(DSP_VOL_MAX,Data);
	Dsp_Single(DSP_CHANNEL_ALL_NONE,VolList[VolData]);
}
void Dsp_SlowCloseSingle(void)
{
	static SCH_U16 Timer;
	if(Timer++%T72MS_8)
		return;
	if(SysPower.nPowerState == POWER_ACCOFF)
	{
		if(App_Dsp.SlowVol)
		{
			App_Dsp.SlowVol--;
			Dsp_Single_A(DSP_CHANNEL_ALL_NONE,VolList[App_Dsp.SlowVol]);
		}
	}
	else if(SysPower.nPowerState == POWER_NORMAL_RUN && App_Dsp.SlowVol != 0xFF)
	{
		if(App_Dsp.SlowVol > VolData)
		{
			App_Dsp.SlowVol--;
			Dsp_Single_A(DSP_CHANNEL_ALL_NONE,VolList[App_Dsp.SlowVol]);
		}
		else if(App_Dsp.SlowVol < VolData)
		{
			App_Dsp.SlowVol++;
			Dsp_Single_A(DSP_CHANNEL_ALL_NONE,VolList[App_Dsp.SlowVol]);
		}
		else
		{
			App_Dsp.SlowVol = 0xFF;
		}
	}
	else if(SysPower.nPowerState == POWER_ACCOFF2)
	{
		App_Dsp.SlowVol = 0xFF;
	}
}
///================================================================================================Single END========

///================================================================================================Mute========
const SCH_U16 Mute_addr[9]=
{
	MOD_MUTE1_2_MUTENOSLEWADAU145XALG9MUTE_ADDR,
	MOD_MUTE1_MUTENOSLEWADAU145XALG1MUTE_ADDR,
	MOD_MUTE2_MUTENOSLEWADAU145XALG2MUTE_ADDR,
	MOD_MUTE3_MUTENOSLEWADAU145XALG3MUTE_ADDR,
	MOD_MUTE4_MUTENOSLEWADAU145XALG4MUTE_ADDR,
	MOD_MUTE4_2_MUTENOSLEWADAU145XALG5MUTE_ADDR,
	MOD_MUTE4_3_MUTENOSLEWADAU145XALG6MUTE_ADDR,
	MOD_MUTE4_4_MUTENOSLEWADAU145XALG7MUTE_ADDR,
	MOD_MUTE4_5_MUTENOSLEWADAU145XALG8MUTE_ADDR
};
void Dsp_Mute_A(SCH_U8 Channel,SCH_U8 Data,SCH_U8 Direct)
{
	SCH_U8 buff[4] = {0x00, 0x00, 0x00, 0x00};
	buff[3] = Data;
	if(Channel > DSP_CHANNEL_CNT)
		return;
	if(Data==DSP_MUTE&&Direct==0)
	{
		Dsp_VolUpdate(Channel,SOFTON);
		SysWaitMs(10);
	}
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, Mute_addr[Channel],  4, buff);
	if(Data==DSP_UNMUTE)
	{
		if((Channel!=DSP_CHANNEL_ALL_NONE)&&(App_Dsp.Dsp_Data.Mute[DSP_CHANNEL_ALL_NONE]==DSP_MUTE))
		{
			SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, Mute_addr[DSP_CHANNEL_ALL_NONE],  4, buff);
			App_Dsp.Dsp_Data.Mute[DSP_CHANNEL_ALL_NONE] = DSP_UNMUTE;
		}
		if(Direct==0)
		{
			SysWaitMs(10);
			Dsp_VolUpdate(Channel,SOFTOFF);
		}
	}
}
void Dsp_Mute(SCH_U8 Channel,SCH_U8 data)
{
	MUTE_MODE Data = data ? DSP_UNMUTE : DSP_MUTE;
	Dsp_Mute_A(Channel,Data,0);
	App_Dsp.Dsp_Data.Mute[Channel] = Data;
}
void Dsp_Mute_Direct(SCH_U8 Channel,SCH_U8 data)
{
	MUTE_MODE Data = data ? DSP_UNMUTE : DSP_MUTE;
	Dsp_Mute_A(Channel,Data,1);
	App_Dsp.Dsp_Data.Mute[Channel] = Data;
}
///================================================================================================Mute END========






