/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_spi.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.12.15
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"



#if SPI0_FUNC == SPI_MODE_INT
#define MAX_SPI0_TX_BUF       300
SCH_U8 Spi0_TX_buf[MAX_SPI0_TX_BUF];
QUEUE_T SPI0_TX_QUEUE = {0,0,0,MAX_SPI0_TX_BUF,1,Spi0_TX_buf};
#define MAX_SPI0_RX_BUF       300
SCH_U8 Spi0_RX_buf[MAX_SPI0_RX_BUF];
QUEUE_T SPI0_RX_QUEUE = {0,0,0,MAX_SPI0_RX_BUF,1,Spi0_RX_buf};
#endif
#if SPI1_FUNC == SPI_MODE_INT
#define MAX_SPI1_TX_BUF       10
SCH_U8 Spi1_TX_buf[MAX_SPI1_TX_BUF];
QUEUE_T SPI1_TX_QUEUE = {0,0,0,MAX_SPI1_TX_BUF,1,Spi1_TX_buf};
#define MAX_SPI1_RX_BUF       300
SCH_U8 Spi1_RX_buf[MAX_SPI1_RX_BUF];
QUEUE_T SPI1_RX_QUEUE = {0,0,0,MAX_SPI1_RX_BUF,1,Spi1_RX_buf};
#endif



QUEUE_T *const SpiBufAddr[][2] = 
{
#if SPI0_FUNC == SPI_MODE_INT
	&SPI0_RX_QUEUE,&SPI0_TX_QUEUE,
#else
	NULL,NULL,
#endif
#if SPI1_FUNC == SPI_MODE_INT
	&SPI1_RX_QUEUE,&SPI1_TX_QUEUE,
#else
	NULL,NULL,
#endif
};


SPI_Type *Spi_Arry[]={SPI0,SPI1};

SCH_U8 SPI_RW(Spi_T spi,SCH_U8 Data)
{
	while(SPI_IsSPTEF(Spi_Arry[spi]) == 0);
	SPI_WriteDataReg(Spi_Arry[spi],Data);
	while(SPI_IsSPRF(Spi_Arry[spi]) == 0);
	return SPI_ReadDataReg(Spi_Arry[spi]);
}


void SpiBufInit(Spi_T spi,Spi_RT TxRx)
{
	if(SpiBufAddr[spi][TxRx])
		Queue_Init(SpiBufAddr[spi][TxRx]);
}
SCH_U16 SpiBufCnt(Spi_T spi,Spi_RT TxRx)
{
	QUEUE_T *pSpiBuf;
	pSpiBuf = SpiBufAddr[spi][TxRx];
	if(pSpiBuf)
		return Queue_Cnt(pSpiBuf);
	return 0;
}
SCH_BOOL SpiGetFromBuf(Spi_T spi, Spi_RT TxRx, SCH_U8 *data, SCH_U16 Len)
{
	QUEUE_T *pSpiBuf;
	pSpiBuf = SpiBufAddr[spi][TxRx];
	if(pSpiBuf)
		return Queue_Out(pSpiBuf, data, Len);
	return FALSE;
}
SCH_BOOL SpiPutToBuf(Spi_T spi, Spi_RT TxRx, SCH_U8 *const data, SCH_U16 Len)
{
	QUEUE_T *pSpiBuf;
	pSpiBuf = SpiBufAddr[spi][TxRx];
	if(pSpiBuf)
		return Queue_In(pSpiBuf, data,Len);
	return FALSE;
}
/********************************************************************************
**  Function	: SpiSendData8
**  Author		: wenjunHu
**  Created on	: 20171218
**  Description	: 发送一个8位的数据
**  Return		: void
********************************************************************************/
void SpiSendData8(Spi_T spi,SCH_U8 u8data)
{
	SPI_WriteDataReg(Spi_Arry[spi], u8data); 
}
/********************************************************************************
**  Function	: SpiTxInt En/Dis
**  Author		: wenjunHu
**  Created on	: 20171218
**  Description	:
**  Return		: void
********************************************************************************/
void SpiTxIntEn(Spi_T spi)
{
	SPI_TxIntEnable(Spi_Arry[spi]); 
}
void SpiTxIntDis(Spi_T spi)
{
	SPI_TxIntDisable(Spi_Arry[spi]);
}

/********************************************************************************
**  Function	: SpiRx/TxCnt
**  Author		: wenjunHu
**  Created on	: 20171218
**  Description	:
**  Return		: SCH_U16
********************************************************************************/
SCH_U16 SpiRxCnt(Spi_T spi)
{
	return SpiBufCnt(spi,Spi_Rx);
}
SCH_U16 SpiTxCnt(Spi_T spi)
{
	return SpiBufCnt(spi,Spi_Tx);
}
/********************************************************************************
**  Function    : Spi_Rx_DataPro
**  Author      : wenjunHu
**  Created on  : 20171218
**  Description	:  ---   int函数中执行
**  Return		: NULL
********************************************************************************/
void Spi_Rx_DataPro(Spi_T spi,SCH_U8 data)
{
	SpiPutToBuf(spi,Spi_Rx,&data,1);
}
/********************************************************************************
**  Function    : Spi_Tx_DataPro
**  Author      : wenjunHu
**  Created on  : 20160621
**  Description :   ---   int函数中执行
**  Return      : NULL
********************************************************************************/
void Spi_Tx_DataPro(Spi_T spi)
{
	SCH_U8 u8data;
	if(SpiTxCnt(spi) == 0)
	{
		SpiTxIntDis(spi);
		return;
	}
	if(SpiGetFromBuf(spi,Spi_Tx,&u8data,1))
	{
		SpiSendData8(spi, u8data);
		SpiTxIntEn(spi);
	}
}
/********************************************************************************
**  Function	: SpiRxData
**  Author		: wenjunHu
**  Created on	: 20171218
**  Description	:
**  Return		: BOOL
********************************************************************************/
SCH_BOOL SpiRxData(Spi_T spi, SCH_U8 *data, SCH_U16 Len)
{
	return SpiGetFromBuf(spi,Spi_Rx,data,Len);
}
/********************************************************************************
**  Function         : SpiTxData
**  Author           : wenjunHu
**  Created on       : 20160621
**  Description      :
**  Return           : BOOL
********************************************************************************/
SCH_BOOL SpiTxData(Spi_T spi, SCH_U8 *const data, SCH_U16 Len)
{
	if(SpiTxCnt(spi) != 0)
	{
		if(SpiPutToBuf(spi,Spi_Tx,data,Len))
			return TRUE;
		return FALSE;
	}
	if(SpiPutToBuf(spi,Spi_Tx,data,Len))
	{
		Spi_Tx_DataPro(spi);
		return TRUE;
	}
	return FALSE;
}
SCH_BOOL SpiTxData_Direct(Spi_T spi,const SCH_U8 *data, SCH_U16 Len)
{
	while(Len--)
	{
		while(!SPI_IsSPTEF(Spi_Arry[spi]));
		SpiSendData8(spi,*data);
		data++;
	}
	return TRUE;
}
/********************************************************************************
**  Function    : Spi_IntSerive
**  Author      : wenjunHu
**  Created on  : 20170407
**  Description :  ---   int函数中执行
**  Return      : void
********************************************************************************/
void SPI_IntSerive(Spi_T spi)
{
	SCH_U8 data;
	if(SPI_IsSPRF(Spi_Arry[spi])) /* Rx interrupt */
	{
		data = SPI_ReadDataReg(Spi_Arry[spi]);
		Spi_Rx_DataPro(spi, data);
	}
	if(SPI_IsSPTEF(Spi_Arry[spi]))/* Tx interrupt */
	{
		Spi_Tx_DataPro(spi);
	}
}



/********************************************************************************
**  Function    : SysSpiExit
**  Author      : 
**  Created on  : 20171215
**  Description :
**  Return      : BOOL
********************************************************************************/
void SysSpiExit(Spi_T spi)
{
	SPI_ConfigType sSPIConfig = {0};  
	switch(spi)
	{
		case SCH_Spi0:
			SIM->SCGC &= ~SIM_SCGC_SPI0_MASK;
		    sSPIConfig.sSettings.bModuleEn             = 0;
		    sSPIConfig.sSettings.bMasterMode           = 0;
		    sSPIConfig.sSettings.bClkPhase1            = 0;
		    sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;
		    SPI_Init(SPI0, &sSPIConfig);
			SPI_DeInit(Spi_Arry[spi]);
			break;
		case SCH_Spi1:
			SIM->SCGC &= ~SIM_SCGC_SPI1_MASK;
		    sSPIConfig.sSettings.bModuleEn             = 0;
		    sSPIConfig.sSettings.bMasterMode           = 0;
		    sSPIConfig.sSettings.bClkPhase1            = 0;
		    sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;
		    SPI_Init(SPI1, &sSPIConfig);
			SPI_DeInit(Spi_Arry[spi]);
			break;
		default:break;
	}
}

/********************************************************************************
**  Function    : SysSpiInit
**  Author      : 
**  Created on  : 20171215
**  Description :
**  Return      : BOOL
********************************************************************************/
SCH_BOOL SysSpiInit(Spi_T spi,SCH_U32 Baudrate)
{
	SPI_ConfigType sSPIConfig = {0};  
	switch(spi)
	{
		case SCH_Spi0:
			SIM_RemapSPI0ToPTE_0_12_3();
#if SPI0_FUNC == SPI_MODE_INT
		    sSPIConfig.u32BitRate = Baudrate;
		    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
		    sSPIConfig.sSettings.bModuleEn             = 1;
		    sSPIConfig.sSettings.bMasterMode           = 1;
			sSPIConfig.sSettings.bIntEn                = 1;
			sSPIConfig.sSettings.bTxIntEn              = 1;
		    sSPIConfig.sSettings.bClkPhase1            = 1;
			sSPIConfig.sSettings.bClkPolarityLow       = 1;
		    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1;
		    SPI_Init(SPI0, &sSPIConfig);
    		NVIC_EnableIRQ(SPI0_IRQn);
			NVIC_SetPriority(SPI0_IRQn, (1<<__NVIC_PRIO_BITS) - 4);
#elif SPI0_FUNC == SPI_MODE_UNINT
			sSPIConfig.u32BitRate = Baudrate;
			sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
			sSPIConfig.sSettings.bModuleEn			   = 1;
			sSPIConfig.sSettings.bMasterMode		   = 1;
			sSPIConfig.sSettings.bIntEn 			   = 0;
			sSPIConfig.sSettings.bTxIntEn			   = 0;
			sSPIConfig.sSettings.bClkPhase1 		   = 1;
			sSPIConfig.sSettings.bClkPolarityLow	   = 1;
			sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;
			SPI_Init(SPI0, &sSPIConfig);
#endif
			break;
		case SCH_Spi1:
			SIM_RemapSPI1ToPTG_4_5_6_7();
#if SPI1_FUNC == SPI_MODE_INT
		    sSPIConfig.u32BitRate = Baudrate;
		    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
		    sSPIConfig.sSettings.bModuleEn             = 1;
		    sSPIConfig.sSettings.bMasterMode           = 1;
			sSPIConfig.sSettings.bIntEn                = 1;
			sSPIConfig.sSettings.bTxIntEn              = 0;
		    sSPIConfig.sSettings.bClkPhase1            = 1;
			sSPIConfig.sSettings.bClkPolarityLow       = 1;
		    sSPIConfig.sSettings.bMasterAutoDriveSS    = 1;
		    SPI_Init(SPI1, &sSPIConfig);
    		///NVIC_EnableIRQ(SPI1_IRQn);
#elif SPI1_FUNC == SPI_MODE_UNINT
		    sSPIConfig.u32BitRate = Baudrate;
		    sSPIConfig.u32BusClkHz = BUS_CLK_HZ;
		    sSPIConfig.sSettings.bModuleEn             = 1;
		    sSPIConfig.sSettings.bMasterMode           = 1;
			sSPIConfig.sSettings.bIntEn                = 0;
			sSPIConfig.sSettings.bTxIntEn              = 0;
		    sSPIConfig.sSettings.bClkPhase1            = 1;
			sSPIConfig.sSettings.bClkPolarityLow       = 1;
		    sSPIConfig.sSettings.bMasterAutoDriveSS    = 0;
		    SPI_Init(SPI1, &sSPIConfig);
#endif
			break;
		default:break;
	}		 
	SpiBufInit(spi, Spi_Tx);
	SpiBufInit(spi, Spi_Rx);
	return TRUE;
}

