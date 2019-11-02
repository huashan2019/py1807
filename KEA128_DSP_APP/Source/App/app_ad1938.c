/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_ad1938.c  ---  
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2018.07.20
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"
/********************************************************************************
**  Function	: AD1938
**  Author		: huwenjun
**  Created on	: 20190401
**  Description	:
**  Return		: 
********************************************************************************/
void AD1938_IO_Init(void)
{///===
	GPIO_PinInit(GPIO_AD1938_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(GPIO_AD1978_RESET_CTL, GPIO_PinOutput);
	GPIO_PinInit(SPI_AD1938_SS,         GPIO_PinOutput);///AD1938 片选脚
	CLATCH_HIGH();
}
/********************************************************************************
**  Function	: AD1938_WriteData   AD1938_ReadData
**  Author		: huwenjun
**  Created on	: 20190401
**  Description	:
**  Return		: 
********************************************************************************/
void AD1938_WriteData(SCH_U8 Register, SCH_U8 Data)
{
	CLATCH_LOW();              // CSN置低，开始传输数据
	SPI_RW(Spi_AD1938,AD1938_GLOBAL_ADDR);       
	SPI_RW(Spi_AD1938,Register);   
	SPI_RW(Spi_AD1938,Data);
	CLATCH_HIGH();             // CSN拉高，结束数据传输
	return;             //
}
SCH_U8 AD1938_ReadData(SCH_U8 Register)
{
	SCH_U8 Data;
	CLATCH_LOW();              // CSN置低，开始传输数据
	SPI_RW(Spi_AD1938,AD1938_GLOBAL_ADDR|0x01);       
	SPI_RW(Spi_AD1938,Register);   
	Data = SPI_RW(Spi_AD1938,0xFF); 
	CLATCH_HIGH();             // CSN拉高，结束数据传输
	return Data;             //
}
/********************************************************************************
**  Function	: AD1938_Freq
**  Author		: huwenjun
**  Created on	: 20190401
**  Description	:
**  Return		: 
********************************************************************************/
void AD1938_Freq(FREQ_T Freq)
{
	switch(Freq)
	{
		case NO_FREQ:
			break;
		case FREQ_48K:
			AD1938_WriteData(P_C_Clock_CTL_0,0x80);
			break;
		case FREQ_192K:
			AD1938_WriteData(P_C_Clock_CTL_0,0x80);
			///AD1938_WriteData(P_C_Clock_CTL_1,0x08);
			///AD1938_WriteData(ADC_CTL_0,0x80);
			///AD1938_WriteData(ADC_CTL_2,0x48);
			///AD1938_WriteData(DAC_CTL_0,0x04);
			///AD1938_WriteData(DAC_CTL_1,0x80);
			break;
		default:break;
	}
}

