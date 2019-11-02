///=========================
#ifndef __MW_GPIO_H
#define __MW_GPIO_H

typedef struct 
{
	SCH_U8 IO_Status;
	SCH_U8 IO_SamplingCounter;
}IO_DET_T;

extern SCH_BOOL GPIO_ReadInputPin(GPIO_PinType GPIO_Pin);
///=======================================================================================ACC===========
///#define IO_ACC                  GPIOC
#define GPIO_ACC_DECT           GPIO_PTI6///ACC_DECT
#define ACC_DET_LEVEL           GPIO_ReadInputPin(GPIO_ACC_DECT)
#define ACC_DET_LVON            (HIGH_LEVEL==ACC_DET_LEVEL)
#define ACC_DET_LVOFF           (LOW_LEVEL==ACC_DET_LEVEL)
extern void ACC_Init(void);
extern void ACC_Detect(void);
///===================ACC END============================

///==============================================================================================LED==========      
///#define LED_CTL            (GPIOH->ODR & GPIO_MUTE_CTL)
#define IO_LED                 GPIOA
#define GPIO_LED_CTL      	   GPIO_PTD2//////
#define TurnOn_LED        	   GPIO_PinSet(GPIO_LED_CTL)
#define TurnOff_LED       	   GPIO_PinClear(GPIO_LED_CTL)
extern void LED_Ctl(SCH_BOOL OnOff);
///===================LED END========================= 

///=================================================================================================USB=========
///#define IO_USB                 GPIOC
#define GPIO_USB_OC_DECT          GPIO_PTI6///
#define USB_OC_LEVEL              GPIO_ReadInputPin(GPIO_USB_OC_DECT)
#define USB_OC_LVON               (LOW_LEVEL==USB_OC_LEVEL)
#define USB_OC_LVOFF              (HIGH_LEVEL==USB_OC_LEVEL)
///#define IO_USB                 GPIOA
#define GPIO_USB_EN_CTL           GPIO_PTB4///
#define USB_EN_ON                 GPIO_PinSet(GPIO_USB_EN_CTL)
#define USB_EN_OFF                GPIO_PinClear(GPIO_USB_EN_CTL)
///#define IO_USB1                GPIOB
#define GPIO_USB1_OC_DECT         GPIO_PTE5///
#define USB1_OC_LEVEL             GPIO_ReadInputPin(GPIO_USB1_OC_DECT)
#define USB1_OC_LVON              (LOW_LEVEL==USB1_OC_LEVEL)
#define USB1_OC_LVOFF             (HIGH_LEVEL==USB1_OC_LEVEL)
///#define IO_USB1                GPIOA
#define GPIO_USB1_EN_CTL          GPIO_PTB5///
#define USB1_EN_ON                GPIO_PinSet(GPIO_USB1_EN_CTL)
#define USB1_EN_OFF               GPIO_PinClear(GPIO_USB1_EN_CTL)
///#define IO_USB_POWER           GPIOB
#define GPIO_USB_POWER_CTL        GPIO_PTE6///
#define USB_POWER_ON()            GPIO_PinSet(GPIO_USB_POWER_CTL)
#define USB_POWER_OFF()           GPIO_PinClear(GPIO_USB_POWER_CTL)
///===================USB END============================


///==============================================================================================BT========
///#define DSP_RESET_CTL		    
#define IO_BT_RESET                GPIOA
#define GPIO_BT_RESET_CTL          GPIO_PTA7//////
#define BT_RESET_RELEASE           GPIO_PinSet(GPIO_BT_RESET_CTL)
#define BT_RESET_HOLD              GPIO_PinClear(GPIO_BT_RESET_CTL)
///===================BT END========================================   

///==============================================================================================SYS PWR========      
///#define IO_SYS_POWER             GPIOA
#define GPIO_SYS_POWER_CTL      	GPIO_PTB2//////
#define TurnOn_SYS_POWER        	GPIO_PinSet(GPIO_SYS_POWER_CTL)
#define TurnOff_SYS_POWER       	GPIO_PinClear(GPIO_SYS_POWER_CTL)
extern void SYS_Power_Ctl(SCH_BOOL OnOff);
///========================================SYS PWR END========================= 

///=============================================================================================SYS_3V3=======
#define IO_SYS_3V3              GPIOB
#define GPIO_SYS_3V3_CTL      	GPIO_PTH7//////
#define SYS_3V3_ON          	GPIO_PinSet(GPIO_SYS_3V3_CTL)
#define SYS_3V3_OFF         	GPIO_PinClear(GPIO_SYS_3V3_CTL)
///===================ACC_3V3 END========================= 

///=============================================================================================SYS_12V=======
#define IO_SYS_12V                  GPIOB
#define GPIO_SYS_12V_CTL      	    GPIO_PTE1
#define SYS_12V_ON        	        GPIO_PinSet(GPIO_SYS_12V_CTL)
#define SYS_12V_OFF       	        GPIO_PinClear(GPIO_SYS_12V_CTL)
///===================SYS_12V END========================= 

extern void GPIOInit(void);
#endif
