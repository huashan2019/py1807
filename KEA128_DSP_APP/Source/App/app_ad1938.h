///=========================
#ifndef __APP_AD1938_H
#define __APP_AD1938_H

#define Spi_AD1938        SCH_Spi1

#define SPI_AD1938_SS      GPIO_PTF4
#define CLATCH_HIGH()      GPIO_PinSet(SPI_AD1938_SS)
#define CLATCH_LOW()       GPIO_PinClear(SPI_AD1938_SS)

///#define AD1938_RESET_CTL		    
#define IO_AD1938_RESET             GPIOA
#define GPIO_AD1938_RESET_CTL       GPIO_PTA6//////
#define AD1938_RESET_RELEASE        GPIO_PinSet(GPIO_AD1938_RESET_CTL)
#define AD1938_RESET_HOLD           GPIO_PinClear(GPIO_AD1938_RESET_CTL)

///#define AD1978_RESET_CTL		    
#define IO_AD1978_RESET             GPIOB
#define GPIO_AD1978_RESET_CTL       GPIO_PTF0//////
#define AD1978_RESET_RELEASE        GPIO_PinSet(GPIO_AD1978_RESET_CTL)
#define AD1978_RESET_HOLD           GPIO_PinClear(GPIO_AD1978_RESET_CTL)
extern void AD1938_IO_Init(void);
///=========================================================================================
#define AD1938_GLOBAL_ADDR     (0x04<<1)
///Register Addr
#define P_C_Clock_CTL_0   0x00
#define P_C_Clock_CTL_1   0x01
#define DAC_CTL_0         0x02
#define DAC_CTL_1         0x03
#define DAC_CTL_2         0x04
#define DAC_Channel_Mute  0x05
#define DAC_L1_Vol_Ctl    0x06
#define DAC_R1_Vol_Ctl    0x07
#define DAC_L2_Vol_Ctl    0x08
#define DAC_R2_Vol_Ctl    0x09
#define DAC_L3_Vol_Ctl    0x0A
#define DAC_R3_Vol_Ctl    0x0B
#define DAC_L4_Vol_Ctl    0x0C
#define DAC_R4_Vol_Ctl    0x0D
#define ADC_CTL_0         0x0E
#define ADC_CTL_1         0x0F
#define ADC_CTL_2         0x10

typedef enum
{
	NO_FREQ,
	FREQ_48K,
	FREQ_192K
}FREQ_T;

extern void AD1938_Freq(FREQ_T Freq);
#endif
