///=========================
#ifndef __APP_DSP_DATA_ACCESS_H
#define __APP_DSP_DATA_ACCESS_H






#define DSP_DATA_LENGTH            ((sizeof(Dsp_Data_T)+3)/4)
#define DSP_SPI_DATA_LENGTH        (sizeof(Dsp_Data_T))
#define DSP_SPI_DATA_MAX_LENGTH    4092///(4K-4)





extern void Dsp_StoreLoadPro(void);
#endif
