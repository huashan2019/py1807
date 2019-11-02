/*
***************************************************************************************************
**  Copyright   : 
**  Project     : STM32F1
**  File        : app_panel.c  ---   按键
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

/*
0:只有短按功能
1:有长按功能，但长按按键没有连续功能
2:有长按功能，并且长按按键有连续功能
*/
////=================AD_SW2 config==========================
KEY_INFO AD_SW2_Info;
KEY_AD_DATA AD_SW2_Tab[AD_SW2_MAX_NUM] =
{
	{0x000, 0x5B8, UICC_MUTE,			UICC_MUTE,			0},
	{0x000, 0x6C6, UICC_MODE,			UICC_MODE,			0},
	{0x000, 0xD3A, UICC_VOLUME_UP, 	    UICC_VOLUME_UP, 	2},
	{0x000, 0xA09, UICC_VOLUME_DOWN,	UICC_VOLUME_DOWN,	2},
	{0x000, 0xC5D, UICC_BT_ACPTCALL,	UICC_BT_ACPTCALL,	0},
	{0x000, 0xB4D, UICC_BT_HUNGUPCALL,  UICC_BT_HUNGUPCALL, 0},
	{0x000, 0x8C1, UICC_SKIPF, 		    UICC_FASTF, 		1},
	{0x000, 0x7E3, UICC_SKIPB, 		    UICC_FASTR,		    1},
};
///====================================================================================================================
void PostKeyCode(SCH_U8 KeyCode,SCH_U8 type)
{
	if(SysPower.nPowerState == POWER_NORMAL_RUN&&Get_CCFL_POWER_Flag == OFF)
		PostMessage(MMI_MODULE,UICC_SCREEN_OPEN,NONE);
	if(Get_REV_Flag==OFF)
		PostMessage(MMI_MODULE,KeyCode,type);	
}
///====================================================================================================================
void ClrADC_Data(KEY_AD_DATA *pTable,SCH_U8 Max_Cnt)
{
	SCH_U8 index;
	for(index=0; index < Max_Cnt; index++)
		pTable[index].KeyADC_Data = 0x0000;
}
void SetADC_Data(KEY_AD_DATA *pTable,SCH_U8 Max_Cnt)
{
	SCH_U8 index;
	for(index=0; index < Max_Cnt; index++)
		pTable[index].KeyADC_Data = pTable[index].KeyADC_DefaultData;
}
SCH_BOOL GetKeyNum(UICC_KEY_CODE KeyCode, SCH_U8 *Num, SCH_U8 Max_Cnt, KEY_AD_DATA *pTable)
{
	SCH_U8 index;
	for(index=0;index < Max_Cnt;index++)
	{
		if(KeyCode == pTable[index].ShortKeyCode)
		{
			*Num = index;
			return TRUE;
		}
	}
	return FALSE;
}
SCH_BOOL GetKeyData(SCH_U8 ad_ch,SCH_U16 *Data)
{
	static SCH_U8 index;
	static SCH_U16 ADC_Value[8];
	if(!ADC_GetData(ad_ch,&ADC_Value[index]))
		return FALSE;
	if(IsInData(ADC_Value[index],ADC_DATA_MAX,0x10))
	{
		index = 0x00;
		return FALSE;		
	}
	if(!IsInData(ADC_Value[index],ADC_Value[0],0x10))
	{ 
		index = 0x00;
		return FALSE;
	}
	if(++index >= dim(ADC_Value))
	{
		*Data = GetAverage_U16(ADC_Value,dim(ADC_Value));
		index = 0x00;
		return TRUE;
	}
	return FALSE;
}
///======================================================================================================Init========
void AD_SW2_Init(void)
{
	sch_memset(&AD_SW2_Info,0x00,sizeof(AD_SW2_Info));
	SetADC_Data(AD_SW2_Tab,AD_SW2_MAX_NUM);
}
void PanelInit(void)
{
	AD_SW2_Init();
}
///======================================================================================================Init end====

///=========================================================================================================key scan=====
/********************************************************************************
**  Function      : KeyTimer
**  Author        : wenjunHu
**  Created on    : 20170804
**  Description   : ADC Key DelayTime
**  Return        : void
********************************************************************************/
void KeyTimer(KEY_INFO *KeyInfo)
{
	if(KeyInfo->HoldTime)
	{
		KeyInfo->HoldTime--;
		if(0==KeyInfo->HoldTime)
		{
			if(0==KeyInfo->PanelFlag.field.KeyProperty)
			{
				PostKeyCode(KeyInfo->ShortKeyCode,PANEL);
			}
			else if(1==KeyInfo->PanelFlag.field.KeyProperty)
			{
				PostKeyCode(KeyInfo->LongKeyCode,PANEL);
				KeyInfo->PanelFlag.field.F_HoldPress=HOLD;
			}
			else if(2==KeyInfo->PanelFlag.field.KeyProperty)
			{
				PostKeyCode(KeyInfo->LongKeyCode,PANEL);
				KeyInfo->PanelFlag.field.F_HoldPress=HOLD;
				KeyInfo->HoldTime=T80MS_8;
			} 
		}
	}
}
/********************************************************************************
**  Function      : ClrKeyInfo
**  Author        : wenjunHu
**  Created on    : 20170804
**  Description   : Clear KeyInfo
**  Return        : void
********************************************************************************/
void ClrKeyInfo(KEY_INFO *KeyInfo)
{
	STUDY_STATE StudyState;
	StudyState = KeyInfo->PanelFlag.field.StudyState;
	sch_memset(KeyInfo,0x00,sizeof(KEY_INFO));
	KeyInfo->PanelFlag.field.StudyState = StudyState;
}
/********************************************************************************
**  Function      : KeyScan
**  Author        : wenjunHu
**  Created on    : 20170803
**  Description   : ADC Key State  up and down
**  Return        : void
********************************************************************************/
void KeyScan(KEY_INFO *KeyInfo)
{
	if(KeyInfo->PanelFlag.field.KeyStatus==Key_Down)/////press down
	{
		if(KeyInfo->ShortKeyCode!=KeyInfo->BkKeyCode)
		{
			KeyInfo->BkKeyCode = KeyInfo->ShortKeyCode;
			if(0==KeyInfo->PanelFlag.field.KeyProperty)/////
			{
				KeyInfo->HoldTime=T96MS_8;
				///PostKeyCode(KeyInfo->ShortKeyCode,PANEL);
			}
			else if(1==KeyInfo->PanelFlag.field.KeyProperty||2==KeyInfo->PanelFlag.field.KeyProperty)
			{
				KeyInfo->HoldTime=T1S_8;
				KeyInfo->PanelFlag.field.F_HoldPress=RELEASE;
			}
		}
	}
	else if(KeyInfo->PanelFlag.field.KeyStatus==Key_Up)///UP
	{
		if(2==KeyInfo->PanelFlag.field.KeyProperty||1==KeyInfo->PanelFlag.field.KeyProperty)
		{
			if(RELEASE==KeyInfo->PanelFlag.field.F_HoldPress)
			{
				if(KeyInfo->HoldTime <= (T1S_8-T96MS_8))
					PostKeyCode(KeyInfo->ShortKeyCode,PANEL);
			}
		}
		ClrKeyInfo(KeyInfo);				  
	}	
}
/********************************************************************************
**  Function      : Key_Scan_AD
**  Author        : wenjunHu
**  Created on    : 20170803
**  Description   : ADC Key Function
**  Return        : void
********************************************************************************/
#define PANEL_STEP    50
void Key_Scan_AD(SCH_U8 ad_ch, SCH_U8 Max_Cnt, KEY_AD_DATA *pTable, KEY_INFO *KeyInfo)
{
	SCH_U8 index;			///
    SCH_U16 ADC_Value; 
	if(!ADC_GetData(ad_ch,&ADC_Value))
		return;
	for(index=0;index<Max_Cnt;index++)
    {
		if(IsInData(ADC_Value,pTable[index].KeyADC_Data,PANEL_STEP))
    	{
			KeyInfo->PanelFlag.field.KeyStatus = Key_Down;
			KeyInfo->ShortKeyCode = pTable[index].ShortKeyCode;
			KeyInfo->LongKeyCode  = pTable[index].LongKeyCode;
			KeyInfo->PanelFlag.field.KeyProperty  = pTable[index].KeyProperty;
    		break;
    	}
    }
	if(index == Max_Cnt)
		KeyInfo->PanelFlag.field.KeyStatus = Key_Up;
	KeyScan(KeyInfo);
	KeyTimer(KeyInfo);
}
///=====================================================================================================key scan end=====
///===================================================================================================power key scan=====
/********************************************************************************
**  Function      : Power_Key_Scan
**  Author        : wenjunHu
**  Created on    : 20170804
**  Description   : IO扫描用于power键
**  Return        : void
********************************************************************************/
void Power_Key_Scan(void)
{
	static SCH_U8 Key_Count_Power;
	static SCH_U8 PowerKey_Timer;
	if(PowerKey_Timer!=0)
	{
		PowerKey_Timer--;
		if(0==PowerKey_Timer)
		{
 			PostKeyCode(UICC_POWER_OFF,PANEL);
			return;
		}
	}
	Key_Count_Power>>=1;
	if(POWER_KEY_DET_LVON) 
	{
		Key_Count_Power =Key_Count_Power| 0x80;
	}
	if(0xf0==Key_Count_Power)
	{//
		PowerKey_Timer = (T1S_8+T200MS_8); //1200ms
	}
	else if(0x0f==Key_Count_Power)
	{//
		if(0!=PowerKey_Timer)
		{	
			PostKeyCode(UICC_POWER_ON,PANEL);
			PowerKey_Timer=0;
		}
	}
}
///===============================================================================================power key scan ennd=====
void AD_SW2_Key_Scan(void)
{
	if(AD_SW2_Info.PanelFlag.field.StudyState == STUDY_OK)
	{
		Key_Scan_AD(AD_SW2_CHANNEL,AD_SW2_MAX_NUM,AD_SW2_Tab,&AD_SW2_Info);
	}
	else if(AD_SW2_Info.PanelFlag.field.StudyState == STUDY_ING)
	{
		if(AD_SW2_Info.StudyKeyCode)
		{
			if(GetKeyData(AD_SW2_CHANNEL,&AD_SW2_Tab[AD_SW2_Info.StudyNum].KeyADC_Data))
			{
				PostMessage(ARM_MODULE,M2A_STUDY_KEY_CMD,SCH_WORD(AD_SW2_Info.StudyKeyCode,0x03));
				AD_SW2_Info.StudyKeyCode = NO_KEY;
			}
		}
	}
	else if(AD_SW2_Info.PanelFlag.field.StudyState == NO_STUDY)
	{
		
	}
}
/********************************************************************************
**  Function      : TASK_Key_Pro
**  Author        : 
**  Created on    : 
**  Description   : 
**  Return        : 
********************************************************************************/
void TASK_Key_Pro(void)
{
	if(Get_START_Flag==OFF)
		return;
	///Power_Key_Scan();
	AD_SW2_Key_Scan();
}



