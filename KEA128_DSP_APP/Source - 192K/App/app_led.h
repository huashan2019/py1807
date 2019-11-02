///=========================
#ifndef __APP_LED_H
#define __APP_LED_H

///==============================================================================================LED==========      
///#define LED_CTL            (GPIOH->ODR & GPIO_MUTE_CTL)
#define IO_LED                 GPIOA
#define GPIO_LED_CTL      	   GPIO_PTD2//////
#define TurnOn_LED        	   GPIO_PinSet(GPIO_LED_CTL)
#define TurnOff_LED       	   GPIO_PinClear(GPIO_LED_CTL)
extern void LED_IO_Init(void);
///===================LED END========================= 

extern void TASK_LED_pro(void);


#endif
