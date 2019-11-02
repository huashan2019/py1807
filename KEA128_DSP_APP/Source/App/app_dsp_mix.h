///=========================
#ifndef __APP_DSP_MIX_H
#define __APP_DSP_MIX_H





#define DSP_MIXER_MAX          186
#define DSP_MIXER_DEFAULT      144
#define DSP_MIXER_MIN            0



///USBÊäÈëµ÷½Ú
#define USB_VOL_MAX 40
#define USB_VOL_DEFAULT 40


extern void Dsp_Mix_Init(void);
extern SCH_BOOL Dsp_Mix_Mixer(SCH_U8 Channel,SCH_U8 *data);
extern SCH_BOOL Dsp_USB_Pro(SCH_U8 data);
extern SCH_BOOL Dsp_Mix_Input(SCH_U8 data);

#endif
