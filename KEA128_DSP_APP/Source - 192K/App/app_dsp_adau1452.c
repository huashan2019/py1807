/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_adau1452.c  ---   与dsp_adau1452通讯(SPI)
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.12.15
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
#include "dsp_IC_1.c"

Dsp_T App_Dsp;

const double Fs = 48000;///48K采样率
const double pi = 3.1415926535898;
///===============================================
void Flash_Get_DspNum(void)
{
	SCH_U32 Dsp_Num;
	Flash_Quick_RD(FLASH_DATA_DSPNUM, &Dsp_Num);
	App_Dsp.DspNum = (Dsp_Num <= DSP_MODE_CNT) ? Dsp_Num : 0x00;
}
void Flash_Set_DspNum(void)
{
	Flash_Quick_WR(FLASH_DATA_DSPNUM, (SCH_U32)App_Dsp.DspNum);
}
///===============================================

///=========================================================================================================
void DspDataRxPro(void)
{
	while(SpiRxCnt(Spi_DSP))
	{
		
	}
}
void M2D_TxService(void)
{
	
}
///=======================================================================================================
void SIGMASTUDIOTYPE(double data,SCH_U8 *buff)
{
	SCH_S32 Data_S32;
	double Data_dbe;
	Data_dbe = data*0x1000000;
	Data_S32 = Data_dbe;
	*buff++ = Data_S32>>24;
	*buff++ = Data_S32>>16;
	*buff++ = Data_S32>>8;
	*buff++ = Data_S32;
}
///================================================================================================Delay========
const SCH_U16 Delay_addr[9]=
{
	0x0000,
	MOD_DELAY_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_2_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_3_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_4_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_5_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_6_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_7_DELAY1_ALG0_DELAYAMT_ADDR,
	MOD_DELAY_8_DELAY1_ALG0_DELAYAMT_ADDR
};
void Dsp_Delay_Init(void)
{
	SCH_U16 index;
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		App_Dsp.Dsp_Data.DelayData[index] = 0x00;
	}
}
void Dsp_Delay(SCH_U8 Channel,SCH_U32 data)
{
	SCH_U8 buff[4] = {0x00, 0x00, 0x00, 0x00};
	if(Channel > DSP_CHANNEL_CNT || Channel == 0x00)
		return;
	buff[0]=data>>24;
	buff[1]=data>>16;
	buff[2]=data>>8;
	buff[3]=data;
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, Delay_addr[Channel],   4, buff);
	App_Dsp.Dsp_Data.DelayData[Channel] = data;
}
///================================================================================================Delay END========

///================================================================================================OutPut========
const SCH_U16 OutPutChl_addr[DSP_OUTPUT_CNT+1]=
{
	0x0000,
	MOD_BOARD1_NX1_1_MONOMUXSIGMA300NS1INDEX_ADDR,
	MOD_BOARD1_NX1_1_2_MONOMUXSIGMA300NS2INDEX_ADDR,
	MOD_BOARD1_NX1_1_3_MONOMUXSIGMA300NS3INDEX_ADDR,
	MOD_BOARD1_NX1_1_4_MONOMUXSIGMA300NS4INDEX_ADDR,
	MOD_BOARD1_NX1_1_5_MONOMUXSIGMA300NS5INDEX_ADDR,
	MOD_BOARD1_NX1_1_6_MONOMUXSIGMA300NS6INDEX_ADDR,
	MOD_BOARD1_NX1_1_7_MONOMUXSIGMA300NS7INDEX_ADDR,
	MOD_BOARD1_NX1_1_8_MONOMUXSIGMA300NS8INDEX_ADDR
};
void Dsp_OutPutChl_Init(void)
{
	SCH_U16 index;
	for(index=1;index<(DSP_OUTPUT_CNT+1);index++)
	{
		App_Dsp.Dsp_Data.OutPutChl[index] = (DSP_CHANNEL_T)index;
	}
}
void Dsp_OutPutChl(SCH_U8 OutPut,SCH_U8 data)
{
	SCH_U8 buff[4] = {0x00, 0x00, 0x00, 0x00};
	if(OutPut > DSP_OUTPUT_CNT || OutPut == 0x00)
		return;
	buff[3] = LimitMaxMin(0 , data, DSP_CHANNEL_CNT);
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, OutPutChl_addr[OutPut],  4, buff);
	App_Dsp.Dsp_Data.OutPutChl[OutPut] = (DSP_CHANNEL_T)data;
}
///========================================================================================OutPut END=========

///========================================================================================Rename=========
const SCH_U8 Default_ReName[9][8] =
{
	0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,///空格
	0x43,0x00,0x48,0x00,0x31,0x00,0x00,0x20,///CH1
	0x43,0x00,0x48,0x00,0x32,0x00,0x00,0x20,///CH2
	0x43,0x00,0x48,0x00,0x33,0x00,0x00,0x20,///CH3
	0x43,0x00,0x48,0x00,0x34,0x00,0x00,0x20,///CH4
	0x43,0x00,0x48,0x00,0x35,0x00,0x00,0x20,///CH5
	0x43,0x00,0x48,0x00,0x36,0x00,0x00,0x20,///CH6
	0x43,0x00,0x48,0x00,0x37,0x00,0x00,0x20,///CH7
	0x43,0x00,0x48,0x00,0x38,0x00,0x00,0x20,///CH8
};
void DspModeNameInit(void)
{
	SCH_U8 index;
	for(index=0x00; index<DSP_MODE_CNT; index++)
	{
		sch_memcpy(App_Dsp.Dsp_ModeName.Name[index],Default_ReName[0],DSP_NAME_SIZE);
	}	
}
void Dsp_ReName_Init(void)
{
	SCH_U16 index;	
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		sch_memcpy(App_Dsp.Dsp_Data.ReName[index],Default_ReName[index],DSP_NAME_SIZE);
	}
}
void Dsp_ReName(SCH_U8 Channel,SCH_U8 *data)
{
	if(Channel > DSP_CHANNEL_CNT)
		return;
	sch_memcpy(App_Dsp.Dsp_Data.ReName[Channel],data,DSP_NAME_SIZE);
}
///========================================================================================Rename END=========

///=======================================================================================Unite=======
SCH_BOOL Dsp_Unite(SCH_U8 Channel0,SCH_U8 Channel1)
{
	SCH_U16 index;
	Filters_T Filter;
	if((Channel0 == Channel1)
		||(Channel0 > DSP_CHANNEL_CNT)
		||(Channel1 > DSP_CHANNEL_CNT)
		||(Channel0 == DSP_CHANNEL_ALL_NONE)
		||(Channel1 == DSP_CHANNEL_ALL_NONE))
		return FALSE;
	Filter = App_Dsp.Dsp_Data.FiltersData[Channel0-1][HIGH_PASS_FILTER];
	Filter.Other.bit.PHASE = App_Dsp.Dsp_Data.FiltersData[Channel1-1][HIGH_PASS_FILTER].Other.bit.PHASE;
	Dsp_GEN_Filter(Channel1-1,HIGH_PASS_FILTER,&Filter);
	Dsp_GEN_Filter(Channel1-1,LOW_PASS_FILTER, &App_Dsp.Dsp_Data.FiltersData[Channel0-1][LOW_PASS_FILTER]);
	for(index=0;index<EQ_NUM_CNT;index++)
	{
		FeedDog();
		Dsp_EQ_Set(Channel1-1,(EQ_NUM_T)index,&App_Dsp.Dsp_Data.EQ_Data[Channel0-1][index]);
	}
	Dsp_Single(Channel1,App_Dsp.Dsp_Data.SingleData[Channel0]);
	return TRUE;
}
///==========================================================================================Unite END============

///=======================================================================================Dsp=======
void DspDataInit(void)
{	
	Dsp_ReName_Init();
	Dsp_Mix_Init();
	Dsp_Filter_Init();
	Dsp_EQ_Init();
	Dsp_Delay_Init();
	Dsp_Single_Mute_Init();
	Dsp_OutPutChl_Init();
}
void Dsp_Updata(void)
{
	SCH_U16 index,index0;
	AudioMute(HARDON);
	TurnOff_REM_EN;
	for(index=1;index<(DSP_CHANNEL_CNT+1);index++)
	{
		Dsp_Mix_Mixer(index,&App_Dsp.Dsp_Data.MixData[index][1]);
	}
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		FeedDog();
		Dsp_GEN_Filter(index,HIGH_PASS_FILTER,&App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER]);
		Dsp_GEN_Filter(index,LOW_PASS_FILTER, &App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER]);
	}
	for(index=1;index<(DSP_CHANNEL_CNT+1);index++)
	{
		Dsp_Delay(index,App_Dsp.Dsp_Data.DelayData[index]);
	}
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		Dsp_Single(index,App_Dsp.Dsp_Data.SingleData[index]);
	}
	for(index=0;index<(DSP_CHANNEL_CNT+1);index++)
	{
		Dsp_Mute_Direct(index,App_Dsp.Dsp_Data.Mute[index]);
	}
	for(index=0;index<(DSP_OUTPUT_CNT+1);index++)
	{
		Dsp_OutPutChl(index,App_Dsp.Dsp_Data.OutPutChl[index]);
	}
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		for(index0=0;index0<EQ_NUM_CNT;index0++)
		{
			FeedDog();
			Dsp_EQ_Set(index,(EQ_NUM_T)index0,&App_Dsp.Dsp_Data.EQ_Data[index][index0]);
		}
	}
	CheckVol();
	TurnOn_REM_EN;
	AudioMute(HARDOFF);
}
///=======================================================================================Dsp END=======
void Dsp_Data_Reset(void)
{
	App_Dsp.DspNum = 0x00;
	Flash_Set_DspNum();
	DspDataInit();
	Dsp_Updata();
	PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
	PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///刷新
}
void Dsp_Data_TestMode(void)
{
	SCH_U16 index;
	App_Dsp.DspNum = 0x00;
	Flash_Set_DspNum();
	DspDataInit();
	App_Dsp.Dsp_Data.SingleData[DSP_CHANNEL_ALL_NONE] = DSP_SINGLE_MAX;	
	App_Dsp.Dsp_Data.SingleMaxData = DSP_SINGLE_MAX;
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		App_Dsp.Dsp_Data.FiltersData[index][HIGH_PASS_FILTER].Other.byte = 0x00;
		App_Dsp.Dsp_Data.FiltersData[index][LOW_PASS_FILTER].Other.byte  = 0x00;
	}
	Dsp_Updata();
	for(index=0;index<DSP_CHANNEL_CNT;index++)
	{
		FeedDog();
		Dsp_EQ_Direct(index+1,SCH_ENABLE);
	}
	PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
	PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///刷新
}
///===================================================================================================
void Dsp_PowerOnInit(void)
{
	switch(App_Dsp.DspPwrState)
	{
		case DSP_INIT_IDLE:
			break;
		case DSP_POWER_EN:
			default_download_IC_1();
			Dsp_USB_Pro(App_Dsp.Dsp_USB_Vol);
			Flash_Get_DspNum();
			if(App_Dsp.DspNum)
			{
				App_Dsp.DSP_Updata_State = Flash_TO_Curr;
				App_Dsp.DspUpdataNum = App_Dsp.DspNum;
			}
			else
			{
				///Dsp_Updata();
				PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
			}
			App_Dsp.DspPwrState = DSP_INIT;
			break;
		case DSP_INIT:
			if(App_Dsp.DSP_Updata_State == UpData_Idle)
			{
				App_Dsp.DSP_Updata_State = Load_Mode_Name;
				///PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0x00,0x00));
				PostMessage(BT_MODULE,M2A_SYS_CMD, M2A_MCU_VER);
				if(App_Dsp.DspNum==0x00)
					PostMessage(BT_MODULE,M2B_DSP_DATA,SCH_WORD(0xFE,0x00));///刷新
				App_Dsp.DspPwrState = DSP_NORMAL;
			}
			break;
		case DSP_NORMAL:
			break; 
		case DSP_CLOSE:
			///App_Dsp.DSP_Updata_State = Store_Mode_Name;
			App_Dsp.DspPwrState = DSP_POWER_DIS;
			break;
		case DSP_POWER_DIS:
			App_Dsp.DspPwrState = DSP_INIT_IDLE;
			break;
		default:break;
	}
}
///==================================================================================
void DspInit(void)
{
	App_Dsp.Dsp_USB_Vol = USB_VOL_DEFAULT;
	App_Dsp.DspNum = 0x00;
	App_Dsp.RequestEQ_Left = 0x0000;
	App_Dsp.RequestEQ_Right = 0x0000;
	App_Dsp.DspPwrState = DSP_INIT_IDLE;
	App_Dsp.DSP_Updata_State = UpData_Idle;
	App_Dsp.SlowVol = 0xFF;
	DspModeNameInit();
	DspDataInit();
}
///===================================================================================
void TASK_Dsp_Pro(void)
{
	Dsp_PowerOnInit();
	Dsp_EQ_Left_Req();
	Dsp_EQ_Right_Req();
	Dsp_StoreLoadPro();
	Dsp_Info_Det();
	Dsp_SlowCloseSingle();
	///M2D_TxService();
}


