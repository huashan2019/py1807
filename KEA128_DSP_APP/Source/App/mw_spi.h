///=========================
#ifndef __MW_SPI_H
#define __MW_SPI_H

#define SPI_DISABLE       0
#define SPI_MODE_INT      1 ///中断模式
#define SPI_MODE_UNINT    2 ///非中断模式

#define SPI0_FUNC    SPI_MODE_UNINT
#define SPI1_FUNC    SPI_MODE_UNINT

typedef enum
{
	SCH_Spi0,
	SCH_Spi1,
	SCH_Spi_Num
}Spi_T;
typedef enum
{
	Spi_Rx,
	Spi_Tx
}Spi_RT;
///============================================================================================
extern SCH_U8 SPI_RW(Spi_T spi,SCH_U8 Data);
extern SCH_BOOL SpiPutToBuf(Spi_T spi, Spi_RT TxRx, SCH_U8 *const data, SCH_U16 Len);
extern void SpiTxIntEn(Spi_T spi);
extern void SpiBufInit(Spi_T spi,Spi_RT TxRx);
extern SCH_U16 SpiTxCnt(Spi_T uart);
extern SCH_BOOL SpiTxData(Spi_T spi, SCH_U8 *const data, SCH_U16 Len);
extern SCH_BOOL SpiTxData_Direct(Spi_T spi,const SCH_U8 *data, SCH_U16 Len);
extern SCH_U16 SpiRxCnt(Spi_T spi);
extern SCH_BOOL SpiRxData(Spi_T spi, SCH_U8 *data, SCH_U16 Len);
extern void SysSpiExit(Spi_T spi);
extern SCH_BOOL SysSpiInit(Spi_T spi,SCH_U32 Baudrate);
extern void SPI_IntSerive(Spi_T spi);
#endif
