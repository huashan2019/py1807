///=========================
#ifndef __APP_MUTE_H
#define __APP_MUTE_H
///==============================================================================================MUTE==========      
///#define MUTE_CTL            (GPIOH->ODR & GPIO_MUTE_CTL)
#define IO_MUTE                GPIOB
#define GPIO_MUTE_CTL      	   GPIO_PTE2//////
#define TurnOn_MUTE        	   GPIO_PinSet(GPIO_MUTE_CTL)
#define TurnOff_MUTE       	   GPIO_PinClear(GPIO_MUTE_CTL)
extern void MUTE_IO_Init(void);
extern void MUTE_Ctl(SCH_BOOL OnOff);
///===================MUTE END========================= 
typedef enum  
{
	SOFTON,
	SOFTOFF,
	HARDON,
	HARDOFF
}MUTE_T;

typedef enum
{
	MUTE_IDLE_STATE,
	MUTE_REVERSING,
	WAIT_REVERSE_END,
	MUTE_UNMUTE_STATE
}MUTE_STATE;
extern MUTE_STATE Mute_State;
extern void AudioMute(MUTE_T MUTE_Type);
extern void TASK_Mute_Pro(void);

#endif
