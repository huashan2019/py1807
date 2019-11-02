///=========================
#ifndef __MW_ADC_H
#define __MW_ADC_H


#define ADC_DATA_MIN     0x0000
#define ADC_DATA_MAX     0x0FFF





extern SCH_BOOL ADC_GetData(SCH_U8 u8Channel,SCH_U16 *ADC_Data);
extern void SysAdcExit(void);
extern void SysAdcInit(void);
#endif
