///=========================
#ifndef __APP_DSP_FILTER_H
#define __APP_DSP_FILTER_H




#define DSP_FREQ_FILTER_MAX  20000
#define DSP_FREQ_FILTER_MIN     20




extern void Dsp_Filter_Init(void);
extern SCH_BOOL Dsp_GEN_Filter(SCH_U8 Channel,Filter_HPLP HPLP,Filters_T *P_Filter);
extern SCH_BOOL Dsp_Filter_Reset(SCH_U8 Channel);




#endif
