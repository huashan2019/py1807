///=========================
#ifndef __APP_DSP_SINGLE_MUTE_H
#define __APP_DSP_SINGLE_MUTE_H



#define DSP_SINGLE_MAX           8000
#define DSP_SINGLE_ALL_MAX       8000///-0db 
#define DSP_SINGLE_ALL_DEFAULT   8000///-0db
#define DSP_SINGLE_DEFAULT       8000
#define DSP_SINGLE_MIN           0






#define DSP_VOL_MAX        40
#define DSP_VOL_DEFAULT    30
extern SCH_U8 VolData;
extern void Dsp_Mute_Init(void);
extern void Dsp_Single_Init(void);
extern void Dsp_Single(SCH_U8 Channel,SCH_U16 data);
extern void Dsp_VolUpdate(SCH_U8 Channel,MUTE_T MUTE_Type);
extern void CheckVol(void);
extern void Vol_Ctl(SCH_U8 Data);
extern void Dsp_SlowCloseSingle(void);

extern void Dsp_Mute_A(SCH_U8 Channel,SCH_U8 Data,SCH_U8 Direct);
extern void Dsp_Mute(SCH_U8 Channel,SCH_U8 data);
extern void Dsp_Mute_Direct(SCH_U8 Channel,SCH_U8 data);

#endif
