/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_eprom.c  ---   ÓëepromÍ¨Ñ¶
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.12.15
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

Eeprom_T App_Eeprom = {EEPROM_IDLE,0x00,0x00,0x00};

/* MX25L6445E 67,108,864 bits  --  8388608 byte(0x800000byte)  --  8192K byte(0x2000k byte)  --  8M byte
+--------------------------------------------------------------------+
| Data                          |  8M|  0x00 0000   ---    0x80 0000 |
+--------------------------------------------------------------------+
*/
#define FLASH_SPI_SECTOR_SIZE               0x1000///4K

#define FLASH_WREN       0x06
#define FLASH_WRDI       0x04
#define FLASH_RDID       0x9F
#define FLASH_RDSR       0x05
#define FLASH_WRSR       0x01
#define FLASH_FASTDTRD   0x0D
#define FLASH_2DTRD      0xBD
#define FLASH_4DTRD      0xED
#define FLASH_READ       0x03
#define FLASH_FASTREAD   0x0B
#define FLASH_RDSFDP     0x5A
#define FLASH_2READ      0xBB
#define FLASH_4READ      0xEB
#define FLASH_4PP        0x38
#define FLASH_SE         0x20///4K byte
#define FLASH_BE64K      0xD8
#define FLASH_BE32K      0x52
#define FLASH_CE0        0x60
#define FLASH_CE1        0xC7
#define FLASH_PP         0x02///256 byte
#define FLASH_CP         0xAD
#define FLASH_DP         0xB9
#define FLASH_RDP        0xAB
#define FLASH_RES        0xAB
#define FLASH_REMS       0x90
#define FLASH_REMS2      0xEF
#define FLASH_REMS4      0xDF
#define FLASH_REMS4D     0xCF
///=============================================
void Eprom_IO_Init(void)
{
	GPIO_PinInit(SPI_EEPROM_CS, GPIO_PinOutput);
	EEPROM_CS_HIGH();
}
///=====================================
void Eprom_WR_Order(SCH_U8 Ctl)
{
	EEPROM_CS_LOW();
	SPI_RW(Spi_FLASH,Ctl);
	EEPROM_CS_HIGH();
}
void Eprom_WR_Command(SCH_U8 Ctl, SCH_U32 addr)
{
	EEPROM_CS_LOW();
	SPI_RW(Spi_FLASH,Ctl);
	SPI_RW(Spi_FLASH,addr>>16);
	SPI_RW(Spi_FLASH,addr>>8);
	SPI_RW(Spi_FLASH,addr);
	EEPROM_CS_HIGH();
}
void Eprom_SE_Data(SCH_U8 Ctl, SCH_U32 addr, SCH_U8 Data,SCH_U16 length)
{
	EEPROM_CS_LOW();
	SPI_RW(Spi_FLASH,Ctl);
	SPI_RW(Spi_FLASH,addr>>16);
	SPI_RW(Spi_FLASH,addr>>8);
	SPI_RW(Spi_FLASH,addr);
	while(length--)
	{
		SPI_RW(Spi_FLASH,Data);
	}
	EEPROM_CS_HIGH();
}
void Eprom_WR_Data(SCH_U8 Ctl, SCH_U32 addr, SCH_U8 *pData,SCH_U16 length)
{
	EEPROM_CS_LOW();
	SPI_RW(Spi_FLASH,Ctl);
	SPI_RW(Spi_FLASH,addr>>16);
	SPI_RW(Spi_FLASH,addr>>8);
	SPI_RW(Spi_FLASH,addr);
	while(length--)
	{
		SPI_RW(Spi_FLASH,*pData++);
	}
	EEPROM_CS_HIGH();
}
void Eprom_RD_Data(SCH_U8 Ctl, SCH_U32 addr, SCH_U8 *pData,SCH_U16 length)
{
	EEPROM_CS_LOW();
	SPI_RW(Spi_FLASH,Ctl);
	SPI_RW(Spi_FLASH,addr>>16);
	SPI_RW(Spi_FLASH,addr>>8);
	SPI_RW(Spi_FLASH,addr);
	while(length--)
	{
		*pData++ = SPI_RW(Spi_FLASH,0xFF);
	}
	EEPROM_CS_HIGH();
}
///=======================================================================Flash_Dsp_WR/RD/CP===========
void Flash_WR_State_Clear(void)
{
	WR_STATE = 0;
}
SCH_BOOL Flash_Dsp_WR(SCH_U8 Num, void *data, SCH_U16 Cnt)
{
	static SCH_U8 Flash_Dsp_Timer = 0;
	SCH_U8 *u8data = (SCH_U8 *)data;
	SCH_U16 Tx_Cnt,index;
	SCH_U32 addr;
	if(Flash_Dsp_Timer)
		Flash_Dsp_Timer--;
	if(Flash_Dsp_Timer)
		return FALSE;
	if(WR_STATE/2)
	{
		addr = Num*FLASH_SPI_SECTOR_SIZE + (WR_STATE/2-1)*256;
		u8data = u8data + (WR_STATE/2-1)*256;
		Tx_Cnt = (WR_STATE/2 < (Cnt+255)/256)?256:(Cnt%256);
	}
	else
	{
		addr = Num*FLASH_SPI_SECTOR_SIZE;
	}
	switch(WR_STATE)
	{
		case 0:
			{
				SCH_U32 *CheckSum = (SCH_U32 *)data;
				*CheckSum = 0x00000000;
				u8data = u8data+4;
				for(index=0;index<(Cnt-4);index++)
				{
					*CheckSum += *u8data;
					u8data++;
				}
				Eprom_WR_Order(FLASH_WREN);
				WR_STATE = 1;
			}
			break;
		case 1:
			Eprom_WR_Command(FLASH_SE,addr);
			Flash_Dsp_Timer = T200MS_8;
			WR_STATE = 2;
			break;
		case 2:
		case 4:
		case 6:
		case 8:
		case 10:
		case 12:
		case 14:
		case 16:
		case 18:
		case 20:
		case 22:
		case 24:
		case 26:
		case 28:
		case 30:
		case 32:
			Eprom_WR_Order(FLASH_WREN);
			WR_STATE++;
			break;
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
		case 13:
		case 15:
		case 17:
		case 19:
		case 21:
		case 23:
		case 25:
		case 27:
		case 29:
		case 31:
		case 33:
			Eprom_WR_Data(FLASH_PP,addr,u8data,Tx_Cnt);
			if((WR_STATE/2 == (Cnt+255)/256))
				WR_STATE = 0xFF;
			else
				WR_STATE++;
			Flash_Dsp_Timer = T48MS_8;
			break;
		case 0xFF:
			WR_STATE = 0x00;
			return TRUE;
		default:break;
	}
	return FALSE;
}
void Flash_RD_State_Clear(void)
{
	RD_STATE = 0;
}
SCH_BOOL Flash_Dsp_RD(SCH_U8 Num, void *data, SCH_U16 Cnt)
{
	SCH_U8 *u8data = (SCH_U8 *)data;
	SCH_U16 Tx_Cnt,index;
	SCH_U32 addr;
	addr = Num*FLASH_SPI_SECTOR_SIZE + (RD_STATE/2)*256;
	if(RD_STATE!=0xFF)
		u8data = u8data + (RD_STATE/2)*256;
	Tx_Cnt = ((RD_STATE/2+1) < (Cnt+255)/256)?256:(Cnt%256);
	switch(RD_STATE)
	{
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
		case 10:
		case 12:
		case 14:
		case 16:
		case 18:
		case 20:
		case 22:
		case 24:
		case 26:
		case 28:
		case 30:
			Eprom_WR_Order(FLASH_WREN);
			RD_STATE++;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
		case 13:
		case 15:
		case 17:
		case 19:
		case 21:
		case 23:
		case 25:
		case 27:
		case 29:
		case 31:
			Eprom_RD_Data(FLASH_READ,addr,u8data,Tx_Cnt);
			if((RD_STATE/2+1) == (Cnt+255)/256)
				RD_STATE = 0xFF;
			else
				RD_STATE++;
			break;
		case 0xFF:
			{
				SCH_U32 *CheckSum = (SCH_U32 *)data;
				SCH_U32 checksum = 0x00000000;
				u8data = u8data+4;
				for(index=0;index<(Cnt-4);index++)
				{
					checksum += *u8data;
					u8data ++;
				}
				RD_STATE = 0x00;
				if(*CheckSum == checksum)
					return TRUE;
				else
					return FALSE;
			}
		default:break;
	}
	return FALSE;
}
void Flash_CP_State_Clear(void)
{
	CP_STATE = 0;
}
SCH_BOOL Flash_Dsp_CP(SCH_U8 FromNum, SCH_U8 ToNum)
{
	static SCH_U8 Flash_Dsp_Timer = 0;
	SCH_U32 From_addr,To_addr;
	SCH_U8 Data[256];
	if(Flash_Dsp_Timer)
		Flash_Dsp_Timer--;
	if(Flash_Dsp_Timer)
		return FALSE;
	if(CP_STATE/2)
	{
		From_addr = FromNum*FLASH_SPI_SECTOR_SIZE + (CP_STATE/2-1)*256;
		To_addr   =   ToNum*FLASH_SPI_SECTOR_SIZE + (CP_STATE/2-1)*256;
	}
	else
	{
		To_addr = ToNum*FLASH_SPI_SECTOR_SIZE;;
	}
	switch(CP_STATE)
	{
		case 0:
			Eprom_WR_Order(FLASH_WREN);
			CP_STATE = 1;
			break;
		case 1:
			Eprom_WR_Command(FLASH_SE,To_addr);
			Flash_Dsp_Timer = T200MS_8;
			CP_STATE = 2;
			break;
		case 2:
		case 4:
		case 6:
		case 8:
		case 10:
		case 12:
		case 14:
		case 16:
		case 18:
		case 20:
		case 22:
		case 24:
		case 26:
		case 28:
		case 30:
		case 32:
			Eprom_WR_Order(FLASH_WREN);
			CP_STATE++;
			break;
		case 3:
		case 5:
		case 7:
		case 9:
		case 11:
		case 13:
		case 15:
		case 17:
		case 19:
		case 21:
		case 23:
		case 25:
		case 27:
		case 29:
		case 31:
		case 33:
			Eprom_RD_Data(FLASH_READ,From_addr,Data,256);
			FeedDog();
			SysWaitMs(1);
			Eprom_WR_Data(FLASH_PP,    To_addr,Data,256);
			Flash_Dsp_Timer = T48MS_8;
			CP_STATE++;
			break;
		case 34:
			CP_STATE = 0;
			return TRUE;
		default:break;
	}
	return FALSE;
}
void Flash_Dsp_SE(SCH_U8 Num)
{
	SCH_U8 index;
	SCH_U32 addr;
	addr = Num*FLASH_SPI_SECTOR_SIZE;
	Eprom_WR_Order(FLASH_WREN);
	SysWaitMs(8);
	Eprom_WR_Command(FLASH_SE,addr);
	SysWaitMs(200);
	for(index=0; index<16; index++)
	{
		Eprom_WR_Order(FLASH_WREN);
		SysWaitMs(8);
		Eprom_SE_Data(FLASH_PP, addr, 0xFF, 256);
		SysWaitMs(8);
		addr += 256;
	}
}
///=======================================================================Flash_Dsp_WR/RD/CP END===========
void EepromDataRxPro(void)
{
	switch(App_Eeprom.nEepromState)
	{
		case EEPROM_IDLE:
			break;
		case EEPROM_INSTRUCTION:
			break;
		case EEPROM_ADDRESS:	
			break;
		case EEPROM_DATA:
			break;
		default:break;
	}
}


void TASK_Eeprom_Pro(void)
{
	
}


