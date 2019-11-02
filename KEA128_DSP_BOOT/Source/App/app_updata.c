/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : APP_updata.c  ---   更新升级
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Iap_T Iap;


#define APP_ADDRESS             0x4004
void BootloaderJumpToApp(void)
{	
	__set_MSP(*(__IO uint32_t*)0x00004000); 
	((pfunction)*(SCH_U32 *)APP_ADDRESS)();
	while(1);
}
#define BOOTLOADER_ADDRESS             0x0004
void AppJumpToBootloader(void)
{
	((pfunction)*(SCH_U32 *)BOOTLOADER_ADDRESS)();
	while(1);
}
///===================================================================================================
#define IAP_MODE_ADDRESS             0x1FC00
#define IAP_MODE_DATA                0xAAAA5555///===app=>bootloader
#define RESET_MODE_DATA              0x5555AAAA///===Reset=>bootloader
SCH_BOOL Is_IapMode(void)
{
	///return (*(SCH_U32 *)IAP_MODE_ADDRESS == IAP_MODE_DATA) ? TRUE : FALSE;
	if((*(SCH_U32 *)IAP_MODE_ADDRESS == IAP_MODE_DATA)
		||(*(SCH_U32 *)IAP_MODE_ADDRESS == RESET_MODE_DATA))
		return TRUE;
	return FALSE;
}
SCH_BOOL Is_ResetMode(void)
{
	return (*(SCH_U32 *)IAP_MODE_ADDRESS == RESET_MODE_DATA) ? TRUE : FALSE;
}
void In_IapMode(void)
{
	FLASH_EraseSector(IAP_MODE_ADDRESS);
	FLASH_Program1LongWord(IAP_MODE_ADDRESS, IAP_MODE_DATA);
}
void In_ResetMode(void)
{
	FLASH_EraseSector(IAP_MODE_ADDRESS);
	FLASH_Program1LongWord(IAP_MODE_ADDRESS, RESET_MODE_DATA);
}
void Out_IapMode(void)
{
	FLASH_EraseSector(IAP_MODE_ADDRESS);
}
/*************************************************************************************
**  Function    : IAP_Pro
**  Author		: wenjunHu
**  Created on	: 20170425
**  Description	: 
**  Return		: NULL
**************************************************************************************/
void IAP_Pro(void)
{
	SCH_INT_DISABLE;
	SysFlashInit();
	In_IapMode();
	Iap.IapState = IAP_IDLE;
	In_ResetMode();
	Updata_Init();
	Bsp_UART_Init();
	///AppJumpToBootloader();
	Clr_IAP_Mode;
}


void Updata_Init(void)
{
	sch_memset(&Iap, 0x00, sizeof(Iap));
}


void MCU_Updata_pro(void)
{
	SCH_U8 index;
	switch(Iap.IapState)
	{
		case IAP_IDLE:
			PostMessage(ARM_MODULE,M2A_IAP_CMD,0x01);///请求长度 校验
			Iap.IAP_ResendTime = 0;
			Iap.IapState = IAP_LENGTH_CHECKSUNM;
			break;
		case IAP_LENGTH_CHECKSUNM:
			if(Iap.IAP_Len)
			{
				Iap.IapState = IAP_DATA;
				Iap.IAP_Sn = 0x0000;
				PostMessage(ARM_MODULE,M2A_IAP_CMD,0x02);
			}
			else
			{
				if(Iap.IAP_ResendTime >= 10)
				{
					Iap.IAP_ResendTime = 0;
					PostMessage(ARM_MODULE,M2A_IAP_CMD,0x01);
				}
			}
			break;
		case IAP_DATA:
			if(Iap.IAP_DataOk)
			{
				for(index=0;index < 128;index++)
				{
					Iap.IAP_Checksum1 += Iap.IAP_Data[index];
				}
				if(Iap.IAP_Sn%4==0)
					FLASH_EraseSector(0x00004000+Iap.IAP_Sn/4*512);
				Flash_Program_U8(0x00004000+Iap.IAP_Sn*128,Iap.IAP_Data,128);
				Iap.IAP_DataOk = 0;
				if((Iap.IAP_Sn+1) >= (Iap.IAP_Len+127)/128)
				{
					Iap.IapState = IAP_END;
				}
				else
				{	
					Iap.IAP_Sn++;
					sch_memset(Iap.IAP_Data,0x00,128);
					PostMessage(ARM_MODULE,M2A_IAP_CMD,0x02);
				}
				Iap.IAP_ResendTime = 0;
			}
			else
			{
				if(Iap.IAP_ResendTime >= T400MS_8)
				{
					Iap.IAP_ResendTime = 0;
					PostMessage(ARM_MODULE,M2A_IAP_CMD,0x02);
				}
			}
			break;
		case IAP_END:
			if(Iap.IAP_ResendTime >= T1S_8)
			{
				Iap.IAP_ResendTime = 0;
				if(Iap.IAP_Checksum1 == Iap.IAP_Checksum)
				{
					PostMessage(ARM_MODULE,M2A_IAP_CMD,0x03);
				}
				else
				{
					Updata_Init();
					Iap.IapState = IAP_IDLE;
					break;
				}
			}
			Iap.IapState = IAP_END;
			break;
		default:
			break;
	}
}






