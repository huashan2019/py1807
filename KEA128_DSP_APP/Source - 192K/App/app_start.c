/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_start.c  ---   Æô¶¯·½Ê½
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.01
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
/********************************************************************************
**  Function    : ACC_Detect
**  Author      : wenjunHu
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T AccDetect;
void ACC_Init(void)
{///===ACC
	GPIO_PinInit(GPIO_ACC_DECT, GPIO_PinInput);
	sch_memset(&AccDetect, 0x00, sizeof(AccDetect));
}
void ACC_Detect(void)
{
	AccDetect.IO_Status = ACC_DET_LVON;
	if(AccDetect.IO_Status != Get_ACC_Flag)
	{
		if(++AccDetect.IO_SamplingCounter >= T200MS_8)
		{
			AccDetect.IO_SamplingCounter = 0;
			if(AccDetect.IO_Status)
			{
				Set_ACC_Has;
				Set_ACC_Flag;
				Printf("ACC ON \n");
			}
			else
			{
				Clr_ACC_Flag;
				Printf("ACC OFF \n");
			}
		}	
	}
	else
	{
		AccDetect.IO_SamplingCounter = 0;
	}
}
#if AUDIO_START == ENABLE
/********************************************************************************
**  Function    : AUDIO_Detect
**  Author      : wenjunHu
**  Created on  : 20160909
**  Description :
**  Return      : 
********************************************************************************/
IO_DET_T AudioDetect;
void Audio_Init(void)
{	
	GPIO_PinInit(GPIO_AUDIO_DECT, GPIO_PinInput_InternalPullup);
	sch_memset(&AudioDetect, 0x00, sizeof(AudioDetect));
}
void AUDIO_Detect(void)
{
	AudioDetect.IO_Status = AUDIO_DET_LVON;
	if(AudioDetect.IO_Status != Get_AUDIO_Flag)
	{
		if(++AudioDetect.IO_SamplingCounter >= T200MS_8)
		{
			AudioDetect.IO_SamplingCounter = 0;
			if(AudioDetect.IO_Status)
			{
				Set_AUDIO_Flag;
			}
			else
			{
				Clr_AUDIO_Flag;
			}
		}	
	}
	else
	{
		AudioDetect.IO_SamplingCounter = 0;
	}
}
#endif
///=================================================================================
void Start_IO_Init(void)
{
	ACC_Init();
#if AUDIO_START == ENABLE
	Audio_Init();
#endif
}
void Start_Detect(void)
{
	ACC_Detect();
#if AUDIO_START == ENABLE
	AUDIO_Detect();
#endif
	if((Get_START_Flag)&&(!Get_ACC_Flag)&&(!Get_AUDIO_Flag))
	{
		Clr_START_Flag;
	}
	if((!Get_START_Flag)&&(Get_ACC_Flag||Get_AUDIO_Flag))
	{
		Set_START_Flag;
		PostMessage(POWER_MODULE,EVT_POWER_OFF,POWEROFF_FROM_START);
	}
}


