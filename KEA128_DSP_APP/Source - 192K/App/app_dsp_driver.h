///=========================
#ifndef __APP_DSP_DRIVER_H
#define __APP_DSP_DRIVER_H

#define DEVICE_ADDR_IC_1                          0x0

///=========================================================================DSP IO========  
#define Spi_DSP            SCH_Spi1

#define SPI_DSP_SS         GPIO_PTG7
#define DSP_SS_HIGH()      GPIO_PinSet(SPI_DSP_SS)
#define DSP_SS_LOW()       GPIO_PinClear(SPI_DSP_SS)
///#define DSP_RESET_CTL		    
#define IO_DSP_RESET                GPIOB
#define GPIO_DSP_RESET_CTL          GPIO_PTF1//////
#define DSP_RESET_RELEASE        	GPIO_PinSet(GPIO_DSP_RESET_CTL)
#define DSP_RESET_HOLD              GPIO_PinClear(GPIO_DSP_RESET_CTL) 
///#define IO_REM_EN                GPIOA
#define GPIO_REM_EN_CTL      	    GPIO_PTC2///
#define TurnOn_REM_EN        	    GPIO_PinSet(GPIO_REM_EN_CTL)
#define TurnOff_REM_EN       	    GPIO_PinClear(GPIO_REM_EN_CTL)
extern void DSP_IO_Init(void);
///===================DSP END=============================================================
extern void default_download_IC_1(void);
extern void SIGMA_READ(SCH_U8 devAddress, SCH_U16 address, SCH_U8 *pData);
extern void SIGMA_WRITE_DELAY(SCH_U8 devAddress, SCH_U16 length, const SCH_U8 *pData);
extern void SIGMA_WRITE_REGISTER_BLOCK(SCH_U8 devAddress, SCH_U16 address, SCH_U16 length, const SCH_U8 *pData);
extern void SIGMA_SAFELOAD_WRITE_REGISTER(SCH_U8 devAddress, SCH_U16 address, SCH_U16 length, SCH_U8 *pData);
#endif
