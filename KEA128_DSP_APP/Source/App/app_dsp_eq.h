///=========================
#ifndef __APP_DSP_EQ_H
#define __APP_DSP_EQ_H


#define DSP_BOOST_MAX         4800
#define DSP_BOOST_DEFAULT     2400
#define DSP_BOOST_MIN            0
#define DSP_GAIN_MAX          3000
#define DSP_GAIN_DEFAULT      1500
#define DSP_GAIN_MIN             0
#define DSP_Q_FACTOR_MAX      1600
#define DSP_Q_FACTOR_DEFAULT   141
#define DSP_Q_FACTOR_MIN         0


extern void Dsp_EQ_Init(void);
extern SCH_BOOL Dsp_EQ_Set(SCH_U8 Channel,EQ_NUM_T EQ_NUM,EQ_T *P_EQ);
extern SCH_BOOL Dsp_EQ_Reset(SCH_U8 Channel);
extern SCH_BOOL Dsp_EQ_Direct(SCH_U8 Channel,SCH_U8 EnDis);
extern void Dsp_EQ_Left_Req(void);
extern void Dsp_EQ_Right_Req(void);
#endif
