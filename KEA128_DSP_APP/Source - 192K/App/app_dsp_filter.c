/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_filter.c  ---   ”Îdsp FILTER
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

void Dsp_Filter_Init(void)
{
	SCH_U16 index;
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].Other.byte = 0x00;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].Other.byte  = 0x00;
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].Other.bit.EN_DIS = 1;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].Other.bit.EN_DIS  = 1;
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].MODE0 = MODE0_LINKWITZ;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].MODE0  = MODE0_LINKWITZ;
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].MODE1 = MODE1_24dB_Oct;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].MODE1  = MODE1_24dB_Oct;
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].Freq  = DSP_FREQ_FILTER_MIN;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].Freq   = DSP_FREQ_FILTER_MAX;
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].Gain  = 0x0000;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].Gain   = 0x0000;
	}
}
///================================================================================================Filter========
const SCH_U16 GEN_Filter_addr[DSP_CHANNEL_CNT][2][4]=
{
	{///channel 1
		{///highpass
			MOD_FILTER_GENFILTER1_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_GENFILTER1_2_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_GENFILTER1_3_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_GENFILTER1_4_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_GENFILTER1_5_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_GENFILTER1_6_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_GENFILTER1_7_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_GENFILTER1_8_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 2
		{///highpass
			MOD_FILTER_2_GENFILTER1_9_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_2_GENFILTER1_10_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_2_GENFILTER1_11_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_2_GENFILTER1_12_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_2_GENFILTER1_13_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_2_GENFILTER1_14_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_2_GENFILTER1_15_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_2_GENFILTER1_16_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 3
		{///highpass
			MOD_FILTER_3_GENFILTER1_17_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_3_GENFILTER1_18_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_3_GENFILTER1_19_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_3_GENFILTER1_20_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_3_GENFILTER1_21_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_3_GENFILTER1_22_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_3_GENFILTER1_23_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_3_GENFILTER1_24_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 4
		{///highpass
			MOD_FILTER_4_GENFILTER1_25_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_4_GENFILTER1_26_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_4_GENFILTER1_27_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_4_GENFILTER1_28_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_4_GENFILTER1_29_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_4_GENFILTER1_30_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_4_GENFILTER1_31_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_4_GENFILTER1_32_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 5
		{///highpass
			MOD_FILTER_5_GENFILTER1_25_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_5_GENFILTER1_26_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_5_GENFILTER1_27_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_5_GENFILTER1_28_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_5_GENFILTER1_29_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_5_GENFILTER1_30_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_5_GENFILTER1_31_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_5_GENFILTER1_32_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 6
		{///highpass
			MOD_FILTER_6_GENFILTER1_25_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_6_GENFILTER1_26_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_6_GENFILTER1_27_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_6_GENFILTER1_28_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_6_GENFILTER1_29_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_6_GENFILTER1_30_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_6_GENFILTER1_31_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_6_GENFILTER1_32_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 7
		{///highpass
			MOD_FILTER_7_GENFILTER1_25_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_7_GENFILTER1_26_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_7_GENFILTER1_27_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_7_GENFILTER1_28_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_7_GENFILTER1_29_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_7_GENFILTER1_30_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_7_GENFILTER1_31_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_7_GENFILTER1_32_ALG0_STAGE0_B2_ADDR
		}
	},
	{///channel 8
		{///highpass
			MOD_FILTER_8_GENFILTER1_25_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_8_GENFILTER1_26_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_8_GENFILTER1_27_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_8_GENFILTER1_28_ALG0_STAGE0_B2_ADDR
		},
		{///lowpass
			MOD_FILTER_8_GENFILTER1_29_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_8_GENFILTER1_30_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_8_GENFILTER1_31_ALG0_STAGE0_B2_ADDR,
			MOD_FILTER_8_GENFILTER1_32_ALG0_STAGE0_B2_ADDR
		}
	}
};
void Filter_Bypassed(double *buff_double)
{
	*buff_double++ = 0;  ///b2
	*buff_double++ = 0;  ///b1 
	*buff_double++ = 1;  ///b0 
	*buff_double++ = 0;  ///a2
	*buff_double++ = 0;  ///a1
}
void Filter_Lowpass_Algorithm(double *buff_double, SCH_U32 Freq, SCH_U16 Gain, SCH_U16 Q_Factor)
{
	double gain,Q,f0;
	double w0,alpha,gainLinear;
	double a0;
	gain = ((double)Gain-1500)/100;
	Q = (double)Q_Factor/100;
	f0 = Freq;
	w0 = 2 * pi * f0 /Fs;
	alpha = sin(w0) / (2*Q);
	gainLinear = pow(10,(gain/20));
	a0 = 1 + alpha;
	*buff_double++ = ((1 - cos(w0)) * gainLinear / 2) / a0;     ///b2
	*buff_double++ = ((1 - cos(w0)) * gainLinear) / a0;            ///b1
	*buff_double++ = ((1 - cos(w0)) * gainLinear / 2) / a0;     ///b0
	*buff_double++ = (alpha - 1) / a0;                           ///a2
	*buff_double   = (2 * cos(w0)) / a0;                         ////a1
}
void Filter_Highpass_Algorithm(double *buff_double, SCH_U32 Freq, SCH_U16 Gain, SCH_U16 Q_Factor)
{
	double gain,Q,f0;
	double w0,alpha,gainLinear;
	double a0;
	gain = ((double)Gain-1500)/100;
	Q = (double)Q_Factor/100;
	f0 = Freq;
	w0 = 2 * pi * f0 /Fs;
	alpha = sin(w0) / (2*Q);
	gainLinear = pow(10,(gain/20));
	a0 = 1 + alpha;
	*buff_double++ = (-(1 + cos(w0)) * gainLinear / 2) / a0;     ///b2
	*buff_double++ = ((1 + cos(w0)) * gainLinear) / a0;            ///b1
	*buff_double++ = (-(1 + cos(w0)) * gainLinear / 2) / a0;     ///b0
	*buff_double++ = (alpha - 1) / a0;                           ///a2
	*buff_double   = (2 * cos(w0)) / a0;                         ////a1
}
void Filter_1st_Order_Butterworth(double *buff_double,Filter_HPLP HPLP, SCH_U32 Freq, SCH_U16 Gain)
{
	double gainLinear,omega,sn,cs,a0;
	omega = 2 * pi * Freq /Fs;
	sn = sin(omega);
	cs = cos(omega);
	a0 = sn + cs + 1;
	gainLinear = pow(10,(Gain/20));
	if(HPLP == HIGH_PASS_FILTER)
	{
		*buff_double++ = 0;  ///b2
		*buff_double++ = -gainLinear * (1 + cs) / a0;  ///b1 
		*buff_double++ = gainLinear * (1 + cs) / a0;  ///b0 
		*buff_double++ = 0;  ///a2
		*buff_double   = -(sn - cs - 1) / a0;  ///a1
	}
	else if(HPLP == LOW_PASS_FILTER)
	{
		*buff_double++ = 0;  ///b2
		*buff_double++ = gainLinear * sn / a0;  ///b1 
		*buff_double++ = gainLinear * sn / a0;  ///b0 
		*buff_double++ = 0;  ///a2
		*buff_double   = -(sn - cs - 1) / a0;  ///a1
	}
}
void Filter_2st_Order_Butterworth(double *buff_double,Filter_HPLP HPLP, SCH_U32 Freq, SCH_U16 Gain)
{
	double gainLinear,alpha,omega,sn,cs,a0,f0;	
	f0 = Freq;
	omega = 2 * pi * f0 /Fs;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn / (2 * (1/ sqrt(2)));
	a0 = 1 + alpha; 
	gainLinear = pow(10,(Gain/20));
	if(HPLP == HIGH_PASS_FILTER)
	{
		*buff_double++ =  (1 + cs) / a0 * gainLinear / 2;  ///b2
		*buff_double++ = -(1 + cs) / a0 * gainLinear;      ///b1 
		*buff_double++ =  (1 + cs) / a0 * gainLinear / 2;  ///b0 
		*buff_double++ = -(1 - alpha) / a0;                ///a2
		*buff_double   =  2 * cs / a0;                     ///a1
	}
	else if(HPLP == LOW_PASS_FILTER)
	{
		*buff_double++ =  (1 - cs) / a0 * gainLinear / 2;  ///b2
		*buff_double++ =  (1 - cs) / a0 * gainLinear;      ///b1 
		*buff_double++ =  (1 - cs) / a0 * gainLinear / 2;  ///b0 
		*buff_double++ = -(1 - alpha) / a0;                ///a2
		*buff_double   =  2 * cs / a0;                     ///a1
	}
}
void Filter_Higher_Order_Butterworth(double *buff_double,Filter_HPLP HPLP, SCH_U32 Freq, SCH_U16 Gain, SCH_U8 orderindex,SCH_U8 i)
{
	double gainLinear,alpha,omega,sn,cs,a0,orderangle;
	omega = 2 * pi * Freq /Fs;
	sn = sin(omega);
	cs = cos(omega);
	orderangle = (pi / orderindex) * (i + 0.5);
	alpha = sn / (2 * (1 / (2 * sin(orderangle))));
	a0 = 1 + alpha; 
	gainLinear = pow(10,(Gain/20));	
	if(HPLP == HIGH_PASS_FILTER)
	{
		*buff_double++ =  (1 + cs) / a0 * gainLinear / 2;  ///b2
		*buff_double++ = -(1 + cs) / a0 * gainLinear;      ///b1 
		*buff_double++ =  (1 + cs) / a0 * gainLinear / 2;  ///b0 
		*buff_double++ = -(1 - alpha) / a0;                ///a2
		*buff_double   =  2 * cs / a0;                     ///a1
	}
	else if(HPLP == LOW_PASS_FILTER)
	{
		*buff_double++ =  (1 - cs) / a0 * gainLinear / 2;  ///b2
		*buff_double++ =  (1 - cs) / a0 * gainLinear;      ///b1 
		*buff_double++ =  (1 - cs) / a0 * gainLinear / 2;  ///b0 
		*buff_double++ = -(1 - alpha) / a0;                ///a2
		*buff_double   =  2 * cs / a0;                     ///a1
	}
}
void Filter_2st_Order_Bessel(double *buff_double,Filter_HPLP HPLP, SCH_U32 Freq, SCH_U16 Gain)
{
	double gainLinear,alpha,omega,sn,cs,a0;
	omega = 2 * pi * Freq /Fs;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn / (2 * (1 / sqrt(3)));
	a0 = 1 + alpha; 
	gainLinear = pow(10,(Gain/20));
	if(HPLP == HIGH_PASS_FILTER)
	{
		*buff_double++ =  (1 + cs) / a0 * gainLinear / 2;  ///b2
		*buff_double++ = -(1 + cs) / a0 * gainLinear;      ///b1 
		*buff_double++ =  (1 + cs) / a0 * gainLinear / 2;  ///b0 
		*buff_double++ = -(1 - alpha) / a0;                ///a2
		*buff_double   =  2 * cs / a0;                     ///a1
	}
	else if(HPLP == LOW_PASS_FILTER)
	{
		*buff_double++ =  (1 - cs) / a0 * gainLinear / 2;  ///b2
		*buff_double++ =  (1 - cs) / a0 * gainLinear;      ///b1 
		*buff_double++ =  (1 - cs) / a0 * gainLinear / 2;  ///b0 
		*buff_double++ = -(1 - alpha) / a0;                ///a2
		*buff_double   =  2 * cs / a0;                     ///a1
	}
}
SCH_BOOL Dsp_GEN_Filter(SCH_U8 Channel,Filter_HPLP HPLP,Filters_T *P_Filter)
{
	double buff_double[4][5];
	SCH_U8 index,buff[20] = {0x00};
	SCH_BOOL PHASE_Flag;
	if(Channel >= DSP_CHANNEL_CNT || HPLP>= HPLP_FILTER_CNT)
		return FALSE;
	PHASE_Flag = (P_Filter->Other.bit.PHASE != App_Dsp.Dsp_Data.FiltersData[Channel][HPLP].Other.bit.PHASE) ? TRUE : FALSE;
	P_Filter->Freq = LimitMaxMin(DSP_FREQ_MIN, P_Filter->Freq, DSP_FREQ_MAX);
	if(P_Filter->Other.bit.EN_DIS)//enable
	{
		if((P_Filter->MODE0==MODE0_LINKWITZ)&&(P_Filter->MODE1==MODE1_12dB_Oct))
		{
			Filter_1st_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_1st_Order_Butterworth(buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_LINKWITZ)&&(P_Filter->MODE1==MODE1_24dB_Oct))
		{
			Filter_2st_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_2st_Order_Butterworth(buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_LINKWITZ)&&(P_Filter->MODE1==MODE1_36dB_Oct))
		{
			Filter_Higher_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain,3,0);
			Filter_1st_Order_Butterworth(   buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Higher_Order_Butterworth(buff_double[2],HPLP,P_Filter->Freq,P_Filter->Gain,3,0);
			Filter_1st_Order_Butterworth(   buff_double[3],HPLP,P_Filter->Freq,P_Filter->Gain);
		}
		else if((P_Filter->MODE0==MODE0_LINKWITZ)&&(P_Filter->MODE1==MODE1_48dB_Oct))
		{
			Filter_Higher_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain,4,0);
			Filter_Higher_Order_Butterworth(buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain,4,1);
			Filter_Higher_Order_Butterworth(buff_double[2],HPLP,P_Filter->Freq,P_Filter->Gain,4,0);
			Filter_Higher_Order_Butterworth(buff_double[3],HPLP,P_Filter->Freq,P_Filter->Gain,4,1);
		}
		else if((P_Filter->MODE0==MODE0_BESSEL)&&(P_Filter->MODE1==MODE1_12dB_Oct))
		{
			Filter_2st_Order_Bessel(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[1]);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_BESSEL)&&(P_Filter->MODE1==MODE1_18dB_Oct))
		{
			Filter_2st_Order_Bessel(     buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_1st_Order_Butterworth(buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_BESSEL)&&(P_Filter->MODE1==MODE1_24dB_Oct))
		{
			Filter_2st_Order_Bessel(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_2st_Order_Bessel(buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_BUTTERWORTH)&&(P_Filter->MODE1==MODE1_12dB_Oct))
		{
			Filter_2st_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[1]);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_BUTTERWORTH)&&(P_Filter->MODE1==MODE1_18dB_Oct))
		{
			Filter_Higher_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain,3,0);
			Filter_1st_Order_Butterworth(   buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else if((P_Filter->MODE0==MODE0_BUTTERWORTH)&&(P_Filter->MODE1==MODE1_24dB_Oct))
		{
			Filter_Higher_Order_Butterworth(buff_double[0],HPLP,P_Filter->Freq,P_Filter->Gain,4,0);
			Filter_Higher_Order_Butterworth(buff_double[1],HPLP,P_Filter->Freq,P_Filter->Gain,4,1);
			Filter_Bypassed(buff_double[2]);
			Filter_Bypassed(buff_double[3]);
		}
		else
			return FALSE;
	}
	else///disable
	{
		SysWaitUs(300);
		Filter_Bypassed(buff_double[0]);
		Filter_Bypassed(buff_double[1]);
		Filter_Bypassed(buff_double[2]);
		Filter_Bypassed(buff_double[3]);
	}
	if(P_Filter->Other.bit.PHASE)///∑¥œÚ
	{
		buff_double[0][0] = -buff_double[0][0];
		buff_double[0][1] = -buff_double[0][1];
		buff_double[0][2] = -buff_double[0][2];
	}
	if((PHASE_Flag)&&(App_Dsp.Dsp_Data.Mute[Channel+1]==DSP_UNMUTE))
	{
		Dsp_Mute_A(Channel+1,DSP_MUTE,0);
		SysWaitMs(10);
	}
	for(index=0;index<4;index++)
	{
		SIGMASTUDIOTYPE(buff_double[index][0],&buff[0]);
		SIGMASTUDIOTYPE(buff_double[index][1],&buff[4]);
		SIGMASTUDIOTYPE(buff_double[index][2],&buff[8]);
		SIGMASTUDIOTYPE(buff_double[index][3],&buff[12]);
		SIGMASTUDIOTYPE(buff_double[index][4],&buff[16]);
		SIGMA_SAFELOAD_WRITE_REGISTER(DEVICE_ADDR_IC_1, GEN_Filter_addr[Channel][HPLP][index],   5, buff);
	}
	if((PHASE_Flag)&&(App_Dsp.Dsp_Data.Mute[Channel+1]==DSP_UNMUTE))
	{
		SysWaitMs(30);
		Dsp_Mute_A(Channel+1,DSP_UNMUTE,0);
	}
	App_Dsp.Dsp_Data.FiltersData[Channel][HPLP] = *P_Filter;
	return TRUE;
}
SCH_BOOL Dsp_Filter_Reset(SCH_U8 Channel)
{
	Filters_T Filter;
	if(Channel > DSP_CHANNEL_CNT || Channel == 0x00)
		return FALSE;
	Channel--;
	Filter.Other.byte = 0x00;
	Filter.Other.bit.EN_DIS = 1;
	Filter.MODE0 = MODE0_LINKWITZ;
	Filter.MODE1 = MODE1_24dB_Oct;
	Filter.Freq = DSP_FREQ_FILTER_MIN;
	Filter.Gain = 0x0000;
	Dsp_GEN_Filter(Channel,HIGH_PASS_FILTER,&Filter);
	Filter.Freq = DSP_FREQ_FILTER_MAX;
	Dsp_GEN_Filter(Channel,LOW_PASS_FILTER,&Filter);
	return TRUE;
}
///================================================================================================Filter END========










