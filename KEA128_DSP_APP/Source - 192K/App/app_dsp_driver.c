/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_dsp_driver.c  ---   与dsp驱动
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2019.04.02
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
/********************************************************************************
**  Function	: DSP_IO_Ctl
**  Author		: 
**  Created on	: 20171220
**  Description	:
**  Return		: 
********************************************************************************/
void DSP_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_DSP_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_REM_EN_CTL,    GPIO_PinOutput);
	GPIO_PinInit(SPI_DSP_SS,         GPIO_PinOutput);
	DSP_SS_HIGH();
}
/********************************************************************************
**  Function	: SIGMA_READ SIGMA_WRITE
**  Author		: 
**  Created on	: 20190402
**  Description	: 读写
**  Return		: 
********************************************************************************/
void SIGMA_READ(SCH_U8 devAddress, SCH_U16 address, SCH_U8 *pData)
{
	SCH_U8 length = 4;
	DSP_SS_LOW();
	SPI_RW(Spi_DSP,devAddress);
	SPI_RW(Spi_DSP,address>>8);
	SPI_RW(Spi_DSP,address);
	while(length--)
	{
		*pData++ = SPI_RW(Spi_DSP,0xFF);
	}
	DSP_SS_HIGH();
}
void SIGMA_WRITE(SCH_U8 devAddress, SCH_U16 address, SCH_U8 *pData)
{
	SCH_U8 length = 4;
	DSP_SS_LOW();
	SPI_RW(Spi_DSP,devAddress);
	SPI_RW(Spi_DSP,address>>8);
	SPI_RW(Spi_DSP,address);
	while(length--)
	{
		SPI_RW(Spi_DSP,*pData++);
	}
	DSP_SS_HIGH();
	SysWaitUs(50);
}
/********************************************************************************
**  Function	: SIGMA_SAFELOAD_WRITE_REGISTER
**  Author		: 
**  Created on	: 20190402
**  Description	: 安全写
**  Return		: 
********************************************************************************/
void SIGMA_SAFELOAD_WRITE_REGISTER(SCH_U8 devAddress, SCH_U16 address, SCH_U16 length, SCH_U8 *pData)
{
	SCH_U8 spiBuff[4];
	SCH_U8 index;
	for(index=0;index<length;index++)
	{
		SIGMA_WRITE(devAddress,MOD_SAFELOADMODULE_DATA_SAFELOAD0_ADDR+index,  pData);
		pData+=4;
	}
	spiBuff[0] = 0x00;
	spiBuff[1] = 0x00;
	spiBuff[2] = address>>8;
	spiBuff[3] = address;
	SIGMA_WRITE(devAddress,MOD_SAFELOADMODULE_ADDRESS_SAFELOAD_ADDR,spiBuff);
	spiBuff[0] = 0x00;
	spiBuff[1] = 0x00;
	spiBuff[2] = 0x00;
	spiBuff[3] = length;
	SIGMA_WRITE(devAddress,MOD_SAFELOADMODULE_NUM_SAFELOAD_ADDR,spiBuff);
	SysWaitUs(50);
}
/********************************************************************************
**  Function	: SIGMA_WRITE_REGISTER_BLOCK
**  Author		: 
**  Created on	: 20190402
**  Description	: 直接写
**  Return		: 
********************************************************************************/
void SIGMA_WRITE_REGISTER_BLOCK(SCH_U8 devAddress, SCH_U16 address, SCH_U16 length, const SCH_U8 *pData)
{
	DSP_SS_LOW();
	SPI_RW(Spi_DSP,devAddress);
	SPI_RW(Spi_DSP,address>>8);
	SPI_RW(Spi_DSP,address);
	while(length--)
	{
		SPI_RW(Spi_DSP,*pData++);
	}
	DSP_SS_HIGH();
	SysWaitUs(500);
}

void SIGMA_WRITE_DELAY(SCH_U8 devAddress, SCH_U16 length, const SCH_U8 *pData)
{
	DSP_SS_LOW();
	SPI_RW(Spi_DSP,devAddress);
	while(length--)
	{
		SPI_RW(Spi_DSP,*pData++);
	}
	DSP_SS_HIGH();
	SysWaitUs(500);
}







