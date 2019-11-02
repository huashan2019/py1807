#ifndef __BSP_H
#define __BSP_H








extern void Bsp_Relocation_Int(void);
extern void Bsp_Clock_Init(void);
extern void Bsp_SysTick_Init(void);
extern void Bsp_SysTick_Close(void);
extern void Bsp_GPIO_Init(void);
#ifdef WDOG_EN
extern void Bsp_WDOG_Init(void);
extern void FeedDog(void);
extern void Bsp_WDOG_Close(void);
#else
#define Bsp_WDOG_Init()
#define FeedDog()
#define Bsp_WDOG_Close()
#endif
extern void Bsp_RTC_Init(void);
extern void Bsp_TIM_Init(void);
extern void Bsp_UART_Init(void);
extern void Bsp_SPI_Init(void);
extern void Bsp_CAN_Init(void);
extern void Bsp_ADC_Init(void);
extern void Bsp_I2C_Init(void);
extern void Bsp_FLASH_Init(void);
#endif
