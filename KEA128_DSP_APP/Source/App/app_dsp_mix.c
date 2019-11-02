/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_mix.c  ---   与dsp 检测
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.04
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

void Dsp_Mix_Init(void)
{
	SCH_U16 index,index0;
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		for(index0=0;index0<(DSP_INPUT_CNT+1);index0++)
		{
			if((index==0x00)||(index0==0x00)||(index==index0))
				App_Dsp.Dsp_Data.MixData[index][index0] = DSP_MIXER_DEFAULT;
			else
				App_Dsp.Dsp_Data.MixData[index][index0] = DSP_MIXER_MIN;
		}
	}
}
///================================================================================================Mix========
const SCH_U16 Mix_Mixer_addr[DSP_CHANNEL_CNT+1]=
{
	0x0000,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN00_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN10_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN20_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN30_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN40_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN50_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN60_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1CROSSGAIN70_ADDR
};
SCH_BOOL Dsp_Mix_Mixer(SCH_U8 Channel,SCH_U8 *data)
{
	double Data;
	SCH_U8 index,buff[4] = {0x00, 0x00, 0x00, 0x00};
	if(Channel > DSP_CHANNEL_CNT || Channel == 0x00)
		return FALSE;
	for(index=0;index<DSP_INPUT_CNT;index++)
	{
		*data = LimitMaxMin(DSP_MIXER_MIN,*data,DSP_MIXER_MAX);
		Data = (double)*data-144;
		SIGMASTUDIOTYPE(pow(10,Data/20),buff);
		SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, Mix_Mixer_addr[Channel]+index, 1, buff);
		App_Dsp.Dsp_Data.MixData[Channel][index+1] = *data++;
	}
	return TRUE;
}
///================================================================================================Mix END========
const SCH_U16 Mix_Mixer_Input_addr[DSP_INPUT_CNT]=
{
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN0_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN1_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN2_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN3_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN4_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN5_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN6_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN7_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN8_ADDR,
	MOD_ROUTER1_ALG0_AUDIOSIGNALROUTER32S300ALG1INPUTGAIN9_ADDR
};
///USB输入调节
#define USB_CHANNEL_0    4
#define USB_CHANNEL_1    5
const SCH_U16 UsbVolList[USB_VOL_MAX+1]={
	  0, 
	  1,   9,  17,  30,  36,  40,  46,  50,  54,  56,
	 58,  60,  62,  64,  66,  68,  70,  72,  74,  76,
	 78,  80,  84,  88,  92,  98, 102, 108, 112, 116,
	120, 124, 128, 132, 134, 136, 138, 140, 142, 144,
};
SCH_BOOL Dsp_USB_Pro(SCH_U8 data)
{
	double Data;
	SCH_U8 buff[4] = {0x00, 0x00, 0x00, 0x00};
	Data = (double)UsbVolList[data]-144;
	SIGMASTUDIOTYPE(pow(10,Data/20),buff);
	SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, Mix_Mixer_Input_addr[USB_CHANNEL_0], 1, buff);
	SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, Mix_Mixer_Input_addr[USB_CHANNEL_1], 1, buff);
	App_Dsp.Dsp_USB_Vol = data;
	return TRUE;
}
SCH_BOOL Dsp_Mix_Input(SCH_U8 data)
{
	double Data;
	SCH_U8 index,buff[4] = {0x00, 0x00, 0x00, 0x00};
	for(index=0;index<DSP_INPUT_CNT;index++)
	{
		if(index != USB_CHANNEL_0 && index != USB_CHANNEL_1)
		{
			Data = (double)data-144;
			SIGMASTUDIOTYPE(pow(10,Data/20),buff);
			SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, Mix_Mixer_Input_addr[index], 1, buff);
		}
	}
	return TRUE;
}




