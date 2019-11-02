/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_eq.c  ---   ”Îdsp EQ
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

const SCH_U32 Default_Freq[32] = 
{
	  20,   25,   32,   40,   50,   63,   80,  100,  125,  160,  200,  250,  315,  400,  500, 630,
	 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000,10000,12000,16000,20000
};
void Dsp_EQ_Init(void)
{
	SCH_U16 index,index0;
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		for(index0=0;index0<EQ_NUM_CNT;index0++)
		{
			App_Dsp.Dsp_Data.EQ_Data[index][index0].Other.byte = 0x00;
			App_Dsp.Dsp_Data.EQ_Data[index][index0].Other.bit.EN_DIS = 1;
			App_Dsp.Dsp_Data.EQ_Data[index][index0].MODE0 = PEAKING_EQ;
			App_Dsp.Dsp_Data.EQ_Data[index][index0].Boost = DSP_BOOST_DEFAULT;
			App_Dsp.Dsp_Data.EQ_Data[index][index0].Freq  = Default_Freq[index0];
			App_Dsp.Dsp_Data.EQ_Data[index][index0].Gain  = DSP_GAIN_DEFAULT;
			App_Dsp.Dsp_Data.EQ_Data[index][index0].Q_Factor  = DSP_Q_FACTOR_DEFAULT;
		}
	}
}
///================================================================================================EQ========
const SCH_U16 EQ_Set_addr[16]=
{
	MOD_EQ_MIDFILTER1_ALG0_STAGE0_B2_ADDR,    MOD_EQ_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_2_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_2_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_3_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_3_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_4_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_4_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_5_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_5_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_6_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_6_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_7_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_7_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR,
	MOD_EQ_8_MIDFILTER1_ALG0_STAGE0_B2_ADDR,  MOD_EQ_8_MIDFILTER1_2_ALG0_STAGE0_B2_ADDR
};
void EQ_Algorithm(double *buff_double, SCH_U32 Boost,SCH_U32 Freq,SCH_U16 Gain,SCH_U16 Q_Factor)
{
	double gain,Q,boost,f0;
	double A,w0,alpha,gainLinear;
	double a0;
	gain = ((double)Gain-1500)/100;
	Q = (double)Q_Factor/100;
	boost = ((double)Boost-2400)/100;
	f0 = Freq;
	A = pow(10,(boost/40));	
	w0 = 2 * pi * f0 /Fs;
	alpha = sin(w0) / (2*Q);
	gainLinear = pow(10,(gain/20));
	a0 = 1 + alpha / A;
	*buff_double++ = ((1 - alpha * A) * gainLinear) / a0;     ///b2
	*buff_double++ = (-(2 * cos(w0)) * gainLinear) / a0;    ///b1
	*buff_double++ = ((1 + alpha *A) * gainLinear) / a0;     ///b0
	*buff_double++ = (alpha /A - 1) / a0;                    ///a2
	*buff_double   = (2 * cos(w0)) / a0;                     ////a1
}
SCH_BOOL Dsp_EQ_Set(SCH_U8 Channel,EQ_NUM_T EQ_NUM,EQ_T *P_EQ)
{
	SCH_U16 Addr;
	double buff_double[5];
	SCH_U8 buff[20];
	if(Channel >= DSP_CHANNEL_CNT || EQ_NUM >= EQ_NUM_31)
		return FALSE;
	P_EQ->Boost    = LimitMaxMin(DSP_BOOST_MIN,   P_EQ->Boost,   DSP_BOOST_MAX);
	P_EQ->Freq     = LimitMaxMin(DSP_FREQ_MIN,    P_EQ->Freq,    DSP_FREQ_MAX);
	P_EQ->Gain     = LimitMaxMin(DSP_GAIN_MIN,    P_EQ->Gain,    DSP_GAIN_MAX);
	P_EQ->Q_Factor = LimitMaxMin(DSP_Q_FACTOR_MIN,P_EQ->Q_Factor,DSP_Q_FACTOR_MAX);
	Addr = EQ_Set_addr[Channel*2+EQ_NUM/16]+EQ_NUM%16*5;
	if(P_EQ->Other.bit.EN_DIS)//enable
	{
		if(P_EQ->MODE0==PEAKING_EQ)
		{
			EQ_Algorithm(buff_double,P_EQ->Boost,P_EQ->Freq,P_EQ->Gain,P_EQ->Q_Factor);
		}
		else
			return FALSE;
	}
	else///disable
	{
		SysWaitUs(300);
		buff_double[0] = 0;
		buff_double[1] = 0;
		buff_double[2] = 1;
		buff_double[3] = 0;
		buff_double[4] = 0;
	}
	if(P_EQ->Other.bit.PHASE)///∑¥œÚ
	{
		buff_double[0] = -buff_double[0];
		buff_double[1] = -buff_double[1];
		buff_double[2] = -buff_double[2];
	}
	SIGMASTUDIOTYPE(buff_double[0],&buff[0]);
	SIGMASTUDIOTYPE(buff_double[1],&buff[4]);
	SIGMASTUDIOTYPE(buff_double[2],&buff[8]);
	SIGMASTUDIOTYPE(buff_double[3],&buff[12]);
	SIGMASTUDIOTYPE(buff_double[4],&buff[16]);
	SIGMA_SAFELOAD_WRITE_REGISTER( DEVICE_ADDR_IC_1, Addr,   5, buff);
	App_Dsp.Dsp_Data.EQ_Data[Channel][EQ_NUM] = *P_EQ;
	return TRUE;
}
///===============================================================================================================
SCH_BOOL Dsp_EQ_Reset(SCH_U8 Channel)
{
	SCH_U8 index;
	EQ_T EQ;
	if(Channel > DSP_CHANNEL_CNT || Channel == 0x00)
		return FALSE;
	Channel--;
	EQ.Other.byte = 0x00;
	EQ.Other.bit.EN_DIS = 1;
	EQ.MODE0    = PEAKING_EQ;
	EQ.Boost    = DSP_BOOST_DEFAULT;
	EQ.Gain     = DSP_GAIN_DEFAULT;
	EQ.Q_Factor = DSP_Q_FACTOR_DEFAULT;
	for(index=0;index<EQ_NUM_CNT;index++)
	{
		EQ.Freq = Default_Freq[index];
		Dsp_EQ_Set(Channel,(EQ_NUM_T)index,&EQ);
	}
	return TRUE;
}
SCH_BOOL Dsp_EQ_Direct(SCH_U8 Channel,SCH_U8 EnDis)
{
	SCH_U8 index;
	if(Channel > DSP_CHANNEL_CNT || Channel == 0x00)
		return FALSE;
	Channel--;
	for(index=0;index<EQ_NUM_CNT;index++)
	{
		if(EnDis == SCH_ENABLE)
		{
			SysWaitUs(100);
			App_Dsp.Dsp_Data.EQ_Data[Channel][index].Other.bit.EN_DIS = 0;
		}
		else
		{
			App_Dsp.Dsp_Data.EQ_Data[Channel][index].Other.bit.EN_DIS = 1;
		}
		Dsp_EQ_Set(Channel,(EQ_NUM_T)index,&App_Dsp.Dsp_Data.EQ_Data[Channel][index]);
	}
	return TRUE;
}
///================================================================================================EQ END========

///=================================================================================================
void Dsp_EQ_Left_Req(void)
{
	static SCH_U8 Timer=0;
	if((App_Dsp.RequestEQ_Left&0x8000)==0)
		return;
	Timer++;
	if((Timer >= T32MS_8)&&(Uart_CONNECT == SCH_Uart_PC)
		||(Timer >= T32MS_8)&&(Uart_CONNECT == SCH_Uart_PC1)
		||(Timer >= T64MS_8)&&(Uart_CONNECT == SCH_Uart_BT))
	{
		Timer = 0;
		if(PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(App_Dsp.RequestEQ_Left,0x04)))
		{
			App_Dsp.RequestEQ_Left++;
			if(App_Dsp.RequestEQ_Left%32==0)
				App_Dsp.RequestEQ_Left = 0x0000;
		}
	}
}
void Dsp_EQ_Right_Req(void)
{
	static SCH_U8 Timer=0;
	if((App_Dsp.RequestEQ_Right&0x8000)==0)
		return;
	Timer++;
	if((Timer >= T32MS_8)&&(Uart_CONNECT == SCH_Uart_PC)
		||(Timer >= T32MS_8)&&(Uart_CONNECT == SCH_Uart_PC1)
		||(Timer >= T64MS_8)&&(Uart_CONNECT == SCH_Uart_BT))
	{
		Timer = 0;
		if(PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(App_Dsp.RequestEQ_Right,0x04)))
		{
			App_Dsp.RequestEQ_Right++;
			if(App_Dsp.RequestEQ_Right%32==0)
				App_Dsp.RequestEQ_Right = 0x0000;
		}
	}
}



