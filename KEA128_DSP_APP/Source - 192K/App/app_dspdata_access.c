/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dspdata_access.c  ---   与dsp data存取
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

SCH_BOOL Dsp_Store(SCH_U8 Num)
{
	if(Num == 0)
		return FALSE;	
	if(Flash_Dsp_WR(Num,&App_Dsp,DSP_SPI_DATA_LENGTH)==FALSE)
		return FALSE;
	sch_memcpy(App_Dsp.Dsp_ModeName.Name[Num-1],App_Dsp.Dsp_Data.ReName,DSP_NAME_SIZE);
	return TRUE;
}
SCH_BOOL Dsp_Load(SCH_U8 Num)
{
	if(Num == 0)
		return FALSE;
	if(Flash_Dsp_RD(Num,&App_Dsp,DSP_SPI_DATA_LENGTH)==FALSE)
		return FALSE;
	Dsp_Updata();
	sch_memcpy(App_Dsp.Dsp_ModeName.Name[Num-1],App_Dsp.Dsp_Data.ReName,DSP_NAME_SIZE);
	return TRUE;
}
SCH_BOOL Dsp_Flash_TO_File(SCH_U8 Num,SCH_U8 Clear)
{
	static SCH_U8 State = 0;
	if(Clear == ERROR)
		State = 0;
	switch(State)
	{
		case 0:
			if(Dsp_Load(Num))
			{
				App_Dsp.DspNum = Num;
				Flash_Set_DspNum();
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(0x01,0x00));
				State = 1;
			}
			break;
		case 1:
			if(App_Dsp.DspUpdataNum == 0x00)
			{
				State = 0;
				return TRUE;
			}
			break;
		default:break;
	}
	return FALSE;
}
SCH_BOOL Dsp_File_TO_Flash(SCH_U8 Num)
{
	static SCH_U8 State = 0;
	static SCH_U8 Flash_Dsp_Timer = 0;
	Flash_Dsp_Timer++;
	switch(State)
	{
		case 0:
			PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,0x03));
			Flash_Dsp_Timer = 0x00;
			State = 1;
			break;
		case 1:
			if(App_Dsp.DspUpdataLen)
			{
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,0x04));
				State = 2;
			}
			else if(Flash_Dsp_Timer >= T80MS_8)
			{
				Flash_Dsp_Timer = 0x00;
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,0x03));
			}
			break;
		case 2:
			if(App_Dsp.DspUpdata_DataOk)
			{
				if((App_Dsp.DspUpdataSn+1) >= (App_Dsp.DspUpdataLen+63)/64)
				{
					State = 3;
				}
				else
				{	
					Flash_Dsp_Timer = 0x00;
					App_Dsp.DspUpdataSn++;
					PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,0x04));
				}
				App_Dsp.DspUpdata_DataOk = 0x00;
			}
			else if(Flash_Dsp_Timer >= T400MS_8)
			{
				Flash_Dsp_Timer = 0x00;
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(App_Dsp.DspUpdataNum,0x04));
			}
			break;
		case 3:
			{
				SCH_U32 i,Checksum = 0x00000000;
				SCH_U8 *pData = (SCH_U8 *)(&App_Dsp.Dsp_Data);
				for(i=0;i<App_Dsp.DspUpdataLen;i++)
				{
					Checksum += *pData;
					pData++;
				}
				if(Checksum == App_Dsp.DspUpdataChecksum)
				{
					State = 4;
				}
				else
				{
					State = 0;
					Flash_Dsp_Timer = 0x00;
					App_Dsp.DspUpdataLen = 0x00000000;
					App_Dsp.DspUpdataSn  = 0x00;
					App_Dsp.DspUpdataCnt = 0x00; 
					App_Dsp.DspUpdata_DataOk  = 0x00;
					App_Dsp.DspUpdataChecksum = 0x00;
				}
			}
			break;
		case 4:
			if(Dsp_Store(App_Dsp.DspUpdataNum))
			{
				State = 0;
				Dsp_Updata();
				App_Dsp.DspNum = App_Dsp.DspUpdataNum;
				Flash_Set_DspNum();
				App_Dsp.DspUpdataNum = 0x00;
				App_Dsp.DspUpdataLen = 0x00000000;
				App_Dsp.DspUpdataSn  = 0x00;
				App_Dsp.DspUpdataCnt = 0x00;
				App_Dsp.DspUpdata_DataOk  = 0x00;
				App_Dsp.DspUpdataChecksum = 0x00;
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(0x82,0x00));
				return TRUE;
			}
			break;
		default:break;
	}
	return FALSE;
}
SCH_BOOL Dsp_Load_Mode_Name(void)
{
	if(Flash_Dsp_RD(0x00,&App_Dsp.Dsp_ModeName,sizeof(ModeName_T))==FALSE)
		return FALSE;
	return TRUE;
}
SCH_BOOL Dsp_Store_Mode_Name(void)
{
	if(Flash_Dsp_WR(0x00,&App_Dsp.Dsp_ModeName,sizeof(ModeName_T))==FALSE)
		return FALSE;
	return TRUE;
}
void Dsp_StoreLoadPro(void)
{	
	static SCH_U16 Flash_Dsp_Timer;
	Flash_Dsp_Timer++;
	switch(App_Dsp.DSP_Updata_State)
	{
		case UpData_Idle:
			Flash_Dsp_Timer = T_NOW;
			break;
		case Curr_TO_Flash:///1--存储当前到FLASH  
			if(Flash_Dsp_Timer > T3S_8)
			{
				Flash_WR_State_Clear();
				App_Dsp.DSP_Updata_State = UpData_Idle;
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFF,0x00));///ERROR
			}
			if(Dsp_Store(App_Dsp.DspUpdataNum))
			{
				Flash_Dsp_Timer = T_NOW;
				App_Dsp.DSP_Updata_State = Store_Mode_Name;
				App_Dsp.DspNum = App_Dsp.DspUpdataNum;
				Flash_Set_DspNum();
			}
			break;
		case Flash_TO_Curr:///2--加载FLASH到当前  
			if(Flash_Dsp_Timer > T3S_8)
			{
				Flash_RD_State_Clear();
				App_Dsp.DSP_Updata_State = UpData_Idle;
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFF,0x00));///ERROR
				Dsp_Data_Reset();
			}
			if(Dsp_Load(App_Dsp.DspUpdataNum))
			{
				App_Dsp.DSP_Updata_State = UpData_Idle;
				App_Dsp.DspNum = App_Dsp.DspUpdataNum;
				Flash_Set_DspNum();
				CheckVol();
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///刷新
			}
			break;
		case Flash_TO_File:///3--FLASH存储成FILE
			if(Flash_Dsp_Timer > T8S_8)
			{
				Flash_RD_State_Clear();
				App_Dsp.DSP_Updata_State = UpData_Idle;
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(0xFF,0x00));///ERROR
				Dsp_Flash_TO_File(App_Dsp.DspUpdataNum,ERROR);
			}
			if(Dsp_Flash_TO_File(App_Dsp.DspUpdataNum,NORMAL))
			{
				App_Dsp.DSP_Updata_State = UpData_Idle;
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///刷新
			}
			break;
		case File_TO_Flash:///4--FILE储存成FLASH
			if(Flash_Dsp_Timer > T8S_8)
			{
				Flash_WR_State_Clear();
				App_Dsp.DSP_Updata_State = UpData_Idle;
				PostMessage(BT_MODULE,M2B_DSP_UPDATA,SCH_WORD(0xFF,0x00));///ERROR
			}
			if(Dsp_File_TO_Flash(App_Dsp.DspUpdataNum))
			{
				Flash_Dsp_Timer = T_NOW;
				App_Dsp.DSP_Updata_State = Store_Mode_Name;
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///刷新
			}
			break;
		case Load_Mode_Name:
			if(Flash_Dsp_Timer > T3S_8)///
			{
				Flash_RD_State_Clear();
				DspModeNameInit();
				App_Dsp.DSP_Updata_State = UpData_Idle;
			}
			if(Dsp_Load_Mode_Name())
			{
				App_Dsp.DSP_Updata_State = UpData_Idle;
			}
			break;
		case Store_Mode_Name:
			if(Flash_Dsp_Timer > T3S_8)///
			{
				Flash_WR_State_Clear();
				App_Dsp.DSP_Updata_State = UpData_Idle;
			}
			if(Dsp_Store_Mode_Name())
			{
				App_Dsp.DSP_Updata_State = UpData_Idle;
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
			}		
			break;
		default:break;
	}
}


