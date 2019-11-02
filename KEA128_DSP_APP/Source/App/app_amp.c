/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_amp.c  ---   功放
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.09.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

AMP_T App_Amp;

const SCH_U8 AmpChipAddr[MAX_AMP_TYPE]=
{
	0xdc,
	0xda,
	0xde,
	0xd4,
};
const pfunction AmpDllLoader[MAX_AMP_TYPE]=
{///SCH_NULL
	Amp8541_DLL_Loader,
	Amp8541_DLL_Loader,
	Amp8541_DLL_Loader,
	Amp8541_DLL_Loader
};
void AmpMute(SCH_BOOL OnOff)
{
	if(App_Amp.AmpState==AMP_NORMAL)
	{
		if(App_Amp.pAmpMute)
			App_Amp.pAmpMute(OnOff);
	}
}
SCH_BOOL AmpTypeCheck(void)
{
	SCH_U8 i;
	App_Amp.AmpTypeCheck = FALSE;
	for(i=0;i<MAX_AMP_TYPE;i++)
	{
		if(I2C1_IsFindAddr(AmpChipAddr[i]))
		{
			if(App_Amp.AmpTypeCheck == FALSE)
			{
				App_Amp.AmpAddr = AmpChipAddr[i];
				Printf("AmpAddrCheck OK --- %X \n",App_Amp.AmpAddr);
				App_Amp.AmpTypeCheck = TRUE;
				App_Amp.AmpType = (_AmpSysType_t)i;
			}
			else
			{
				App_Amp.AmpAddr1 = AmpChipAddr[i];
				App_Amp.AmpType1 = (_AmpSysType_t)i;
				Printf("AmpAddrCheck OK --- %X \n",App_Amp.AmpAddr1);
				break;
			}
		}
	}
	if(App_Amp.AmpType == MAX_AMP_TYPE)
	{
		Printf("AmpAddrCheck fail \n");
		return FALSE;
	}
	if(AmpDllLoader[App_Amp.AmpType])
		(*AmpDllLoader[App_Amp.AmpType])();
	return TRUE;
}
/********************************************************************************
**  Function      : TASK_Amp_Pro
**  Author        : 
**  Created on    : 
**  Description   : 16MS执行一次
**  Return        : 
********************************************************************************/
void TASK_Amp_Pro(void)
{
	switch(App_Amp.AmpState)
	{
		case AMP_IDLE:
			break;
		case AMP_ENABLE:
			///if(App_Radio.RadioPwrState == RADIO_NORMAL)
			{
				App_Amp.AmpState = AMP_CHECK;
				TurnOn_AMP;
			}
			break;
		case AMP_CHECK:
			if(AmpTypeCheck())
			{
				if(App_Amp.pAmpInit)
					App_Amp.pAmpInit();
				App_Amp.AmpState = AMP_NORMAL;
			}
			break;
		case AMP_NORMAL:
			break;
		case AMP_DISABLE:
			if(App_Amp.pAmpMute)
				App_Amp.pAmpMute(ON);
			TurnOff_AMP;
			App_Amp.AmpState = AMP_CLOSE;
			break;
		case AMP_CLOSE:
			break;
		default:break;
	}
}

