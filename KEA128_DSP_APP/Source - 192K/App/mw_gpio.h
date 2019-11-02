///=========================
#ifndef __MW_GPIO_H
#define __MW_GPIO_H

typedef struct 
{
	SCH_U8 IO_Status;
	SCH_U8 IO_SamplingCounter;
}IO_DET_T;

extern SCH_BOOL GPIO_ReadInputPin(GPIO_PinType GPIO_Pin);

///=================================================================================================USB=========
///#define IO_USB                 GPIOC
#define GPIO_USB_OC_DECT          GPIO_PTD3///
#define USB_OC_LEVEL              GPIO_ReadInputPin(GPIO_USB_OC_DECT)
#define USB_OC_LVON               (LOW_LEVEL==USB_OC_LEVEL)
#define USB_OC_LVOFF              (HIGH_LEVEL==USB_OC_LEVEL)
///#define IO_USB                 GPIOA
#define GPIO_USB_EN_CTL           GPIO_PTD4///
#define USB_EN_ON                 GPIO_PinSet(GPIO_USB_EN_CTL)
#define USB_EN_OFF                GPIO_PinClear(GPIO_USB_EN_CTL)
///#define IO_USB_POWER           GPIOA
#define GPIO_USB_POWER_CTL        GPIO_PTC5///
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


///==============================================================================================ARM========      
///#define ARM_RESET_CTL		    
#define IO_ARM_RESET                GPIOB
#define GPIO_ARM_RESET_CTL          GPIO_PTG1//////
#define ARM_RESET_RELEASE        	GPIO_PinSet(GPIO_ARM_RESET_CTL)
#define ARM_RESET_HOLD              GPIO_PinClear(GPIO_ARM_RESET_CTL) 	    
///#define ARM_REV_CTL	---   mcu to arm rev	    
#define IO_ARM_REV                  GPIOB
#define GPIO_ARM_REV_CTL            GPIO_PTG2//////
#define ARM_REV_ON             	    GPIO_PinSet(GPIO_ARM_REV_CTL)
#define ARM_REV_OFF       	        GPIO_PinClear(GPIO_ARM_REV_CTL)  
///#define IO_NAVI_GATE             GPIOA
#define GPIO_NAVI_GATE_DECT         GPIO_PTD6///
#define NAVI_GATE_LEVEL             GPIO_ReadInputPin(GPIO_NAVI_GATE_DECT)
#define NAVI_GATE_LVON              (LOW_LEVEL==NAVI_GATE_LEVEL)
#define NAVI_GATE_LVOFF             (HIGH_LEVEL==NAVI_GATE_LEVEL)
///#define IO_ARM_WORK              GPIOA
#define GPIO_ARM_WORK_DECT          GPIO_PTG3///
#define ARM_WORK_LEVEL              GPIO_ReadInputPin(GPIO_ARM_WORK_DECT)
#define ARM_WORK_ERROR              (LOW_LEVEL==ARM_WORK_LEVEL)
#define ARM_WORK_NORMAL             (HIGH_LEVEL==ARM_WORK_LEVEL)
///#define IO_ARM_PWR               GPIOA
#define GPIO_ARM_PWR_CTL      	    GPIO_PTC0///
#define TurnOn_ARM_PWR        	    GPIO_PinSet(GPIO_ARM_PWR_CTL)
#define TurnOff_ARM_PWR       	    GPIO_PinClear(GPIO_ARM_PWR_CTL)
extern void NAVI_GATE_Detect(void);  
extern void ARM_WORK_GATE_Detect(void);
///========================================ARM END========================= 

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

///==========================================================================================AMP===¹¦·Å========      
///#define AMP_CTL		    
#define IO_AMP                GPIOB
#define GPIO_AMP_CTL          GPIO_PTF7///GPIO_PTE5//////
#define TurnOn_AMP        	  GPIO_PinSet(GPIO_AMP_CTL)
#define TurnOff_AMP       	  GPIO_PinClear(GPIO_AMP_CTL)
///#define AMP_BEEP		    
#define IO_AMP_BEEP           GPIOA
#define GPIO_AMP_BEEP         GPIO_PTC5//////
#define TurnOn_AMP_BEEP       GPIO_PinSet(GPIO_AMP_BEEP)
#define TurnOff_AMP_BEEP      GPIO_PinClear(GPIO_AMP_BEEP)
///#define IO_AMP_DET         GPIOB
#define GPIO_AMP_DECT         GPIO_PTB5///
#define AMP_DET_LEVEL         GPIO_ReadInputPin(GPIO_AMP_DECT)
#define AMP_DET_LVON          (LOW_LEVEL==AMP_DET_LEVEL)
#define AMP_DET_LVOFF         (HIGH_LEVEL==AMP_DET_LEVEL)
///#define OPAMP_CTL		    
#define IO_OPAMP                GPIOB
#define GPIO_OPAMP_CTL          GPIO_PTF5//////
#define TurnOn_OPAMP        	  GPIO_PinSet(GPIO_OPAMP_CTL)
#define TurnOff_OPAMP       	  GPIO_PinClear(GPIO_OPAMP_CTL)
///===================AMP END========================= 



extern void GPIOInit(void);
#endif
