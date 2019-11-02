/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_time.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.08.08
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"










////=======================================================================================================FTM====Pwm======
void Sch_FTM_PWMInit(FTM_Type *pFTM, SCH_U8 u8PWMModeSelect, SCH_U8 u8PWMEdgeSelect, SCH_U8 u8FTM_Channel)
{
    ASSERT((FTM0== pFTM) || (FTM1== pFTM) || (FTM2== pFTM));
	if (FTM0 == pFTM)
    {
        SIM->SCGC |= SIM_SCGC_FTM0_MASK;
    }
    else if(FTM1 == pFTM)
    {
#if !defined(CPU_KEA64)
        SIM->SCGC |= SIM_SCGC_FTM1_MASK;
#endif
    }        
    else
    {
        SIM->SCGC  |= SIM_SCGC_FTM2_MASK;
    }
    pFTM->SC  = 0x0;                                    /* disable counter */  
	pFTM->MOD = FTM_MOD_INIT; 
    if(FTM_PWMMODE_CENTERALLIGNED == u8PWMModeSelect)   /* enable CPWM */
    {
        pFTM->SC |= FTM_SC_CPWMS_MASK; 
    }
    else if(FTM_PWMMODE_COMBINE == u8PWMModeSelect)     /* enable combine pwm mode */
    {
        pFTM->MODE    |= FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;
        pFTM->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_DTEN0_MASK |
                        FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK | FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_DTEN1_MASK |
                        FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMP2_MASK | FTM_COMBINE_SYNCEN2_MASK | FTM_COMBINE_DTEN2_MASK ;     
        pFTM->SC &= ~FTM_SC_CPWMS_MASK; 
    }
    if(FTM_PWM_HIGHTRUEPULSE == u8PWMEdgeSelect)
    {
        pFTM->CONTROLS[u8FTM_Channel].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;  
    }
    else if(FTM_PWM_LOWTRUEPULSE == u8PWMEdgeSelect)
    {
        pFTM->CONTROLS[u8FTM_Channel].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK; 
    }  
}
////=======================================================================================================FTM====Pwm======

void FTM0_PWM_Exit(void)////CCFL背光PWM
{
	FTM_DeInit(FTM0);
}
/********************************************************************************
**  Function    : FTM0_PWM_Init
**  Author      : 
**  Created on  : 20170821
**  Description :
**  Return      : BOOL
********************************************************************************/
void FTM0_PWM_Init(void)////CCFL背光PWM
{
	GPIO_PinInit(GPIO_PTB2, GPIO_PinOutput_HighCurrent);
    SIM->PINSEL |= SIM_PINSEL_FTM0PS0_MASK;/////Select FTM0Ch0 on PTB2
    Sch_FTM_PWMInit(FTM0, FTM_PWMMODE_EDGEALLIGNED, FTM_PWM_HIGHTRUEPULSE,FTM_CHANNEL_CHANNEL0); 
    FTM_ClockSet(FTM0, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV8);  
    FTM_SetModValue(FTM0, 5000);///40M/8/5000 = 1000
	FTM_SetChannelValue(FTM0, FTM_CHANNEL_CHANNEL0, 3750);///占空比  =  3750/5000
}







