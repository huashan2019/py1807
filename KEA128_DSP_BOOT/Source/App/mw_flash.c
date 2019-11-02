/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_flash.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.06.30
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"


SCH_U32 u8_TO_u32(SCH_U8 *u8Data)
{
	SCH_U32 u32Data=0x00000000;
	u32Data |= *u8Data++;
	u32Data |= *u8Data++<<8;
	u32Data |= *u8Data++<<16;
	u32Data |= *u8Data<<24;
	return u32Data;
}
/**************************************************************
**  Function    : Flash_Program_U8
**  Author		: wenjunHu
**  Created on	: 20170425
**  Description	: 
**  Return		: NULL
**************************************************************/
SCH_BOOL Flash_Program_U8(SCH_U32 u32NVMTargetAddress,SCH_U8 *pData,SCH_U16 u16SizeBytes)
{
	SCH_U32 u32DwData0,u32DwData1;
	if(u32NVMTargetAddress & 0x03)
	{
		return FALSE;  
	}
	while(u16SizeBytes/8)
	{
		u32DwData0 = u8_TO_u32(pData);
		pData += 4;
		u32DwData1 = u8_TO_u32(pData);
		pData += 4;
		if(FLASH_Program2LongWords(u32NVMTargetAddress, u32DwData0, u32DwData1))
		{
			return FALSE;
		}
		u32NVMTargetAddress += 8;
		u16SizeBytes -= 8;
	}
	while(u16SizeBytes/4)
	{
		u32DwData0 = u8_TO_u32(pData);
		pData += 4;
		if(FLASH_Program1LongWord(u32NVMTargetAddress, u32DwData0))
		{
			return FALSE;
		}
		u32NVMTargetAddress += 4;
		u16SizeBytes -= 4;
	}
	if(u16SizeBytes)
	{
		u32DwData0 = 0x00000000;
		for(u32DwData1 = 0x00;u32DwData1 < 4; u32DwData1++)
		{
			if(u16SizeBytes)
			{
				u32DwData0 |= *pData++<<(8*u32DwData1);
				u16SizeBytes--;
			}
			else
			{
				u32DwData0 |= 0xFF<<(8*u32DwData1);
			}
		}
		if(FLASH_Program1LongWord(u32NVMTargetAddress, u32DwData0))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/**************************************************************
**  Function    : Flash_Program_U32
**  Author		: wenjunHu
**  Created on	: 20170425
**  Description	: 
**  Return		: NULL
**************************************************************/
SCH_BOOL Flash_Program_U32(SCH_U32 u32NVMTargetAddress,SCH_U32 *pData,SCH_U16 u16SizeBytes)
{
	SCH_U32 u32DwData0,u32DwData1;
	if(u32NVMTargetAddress & 0x03)
	{
		return FALSE;  
	}
	while(u16SizeBytes/2)
	{
		u32DwData0 = *pData++;
		u32DwData1 = *pData++;
		if(FLASH_Program2LongWords(u32NVMTargetAddress, u32DwData0, u32DwData1))
		{
			return FALSE;
		}
		u32NVMTargetAddress += 8;
		u16SizeBytes -= 2;
	}
	if(u16SizeBytes)
	{
		u32DwData0 = *pData;
		if(FLASH_Program1LongWord(u32NVMTargetAddress, u32DwData0))
		{
			return FALSE;
		}	
	}
	return TRUE;
}

///========================================================================================================
SCH_BOOL Flash_Read(SCH_U32 u32addr,SCH_U32 *u32data)
{
	if(u32addr & 0x03)
		return FALSE;
	*u32data = *(SCH_U32 *)u32addr;
	return TRUE;
}
SCH_BOOL Flash_Erase(SCH_U32 u32addr)
{
	if(FLASH_EraseSector(u32addr)==FLASH_ERR_SUCCESS)
		return TRUE;
	return FALSE;
}
SCH_BOOL Flash_Write(SCH_U32 u32addr, SCH_U32 u32data)
{
	if(FLASH_Program1LongWord(u32addr,u32data)==FLASH_ERR_SUCCESS)
		return TRUE;
	return FALSE;
}
/**************************************************************
**  Function    : Flash_RD_XXX
**  Author		: wenjunHu
**  Created on	: 20170814
**  Description	: 
**  Return		: NULL
**************************************************************/
SCH_BOOL Flash_RD_NUM(FLASH_DATA flash_data, SCH_U32 *u32data, SCH_U16 num)
{
	if(flash_data+num > FLASH_DATA_MAX)
		return FALSE;
	while(num--)
	{
		Flash_Read(FLASH_DATA_ADDR + flash_data/FLASH_PAGE_CNT*FLASH_PAGE_SIZE + flash_data%FLASH_PAGE_CNT*sizeof(SCH_U32),u32data);
		flash_data++;
		u32data++;
	}
	return TRUE;
}
SCH_BOOL Flash_RD(FLASH_DATA flash_data, SCH_U32 *u32data)
{
	return Flash_RD_NUM(flash_data,u32data,1);
}
/**************************************************************
**  Function    : Flash_WR_XXX
**  Author		: wenjunHu
**  Created on	: 20170814
**  Description	: 
**  Return		: NULL
**************************************************************/
SCH_BOOL Flash_WR_NUM_InOnePiece(FLASH_DATA flash_data, SCH_U32 *u32data, SCH_U16 num)
{
	SCH_U8 count;
	SCH_U32 Data[FLASH_PAGE_CNT];
	if(flash_data%FLASH_PAGE_CNT+num > FLASH_PAGE_CNT)
		return FALSE;
	for(count=0;count<FLASH_PAGE_CNT;count++)
	{
		Flash_Read(FLASH_DATA_ADDR + flash_data/FLASH_PAGE_CNT*FLASH_PAGE_SIZE + count*sizeof(SCH_U32),&Data[count]);
	}
	sch_memcpy(&Data[flash_data%FLASH_PAGE_CNT],u32data,num*sizeof(SCH_U32));
	Flash_Erase(FLASH_DATA_ADDR + flash_data/FLASH_PAGE_CNT*FLASH_PAGE_SIZE);
	for(count=0;count<FLASH_PAGE_CNT;count++)
	{		
		Flash_Write(FLASH_DATA_ADDR + flash_data/FLASH_PAGE_CNT*FLASH_PAGE_SIZE + count*sizeof(SCH_U32),Data[count]);
	}
	return TRUE;
}
SCH_BOOL Flash_WR_NUM(FLASH_DATA flash_data, SCH_U32 *u32data, SCH_U16 num)
{
	SCH_U16 count;
	if(flash_data+num > FLASH_DATA_MAX)
		return FALSE;
	while(num)
	{
		count = GetMin(num,FLASH_PAGE_CNT - flash_data%FLASH_PAGE_CNT);
		Flash_WR_NUM_InOnePiece(flash_data,u32data,count);
		flash_data += count;
		u32data += count;
		num -= count;
	}
	return TRUE;
}
SCH_BOOL Flash_WR(FLASH_DATA flash_data, SCH_U32 u32data)
{
	return Flash_WR_NUM_InOnePiece(flash_data,&u32data,1);
}
/**************************************************************
**  Function    : SysFlashInit
**  Author		: wenjunHu
**  Created on	: 20170814
**  Description	: 
**  Return		: NULL
**************************************************************/
void SysFlashInit(void)
{
    FLASH_Init(BUS_CLK_HZ);
}



