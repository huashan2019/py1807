///=========================
#ifndef __APP_EPROM_H
#define __APP_EPROM_H

/* MX25L6445E 67,108,864 bits  --  8388608 byte(0x800000byte)  --  8192K byte(0x2000k byte)  --  8M byte
+--------------------------------------------------------------------+
| Data                          |  8M|  0x00 0000   ---    0x80 0000 |
+--------------------------------------------------------------------+
*/

#define Spi_FLASH            SCH_Spi0

#define SPI_EEPROM_CS      GPIO_PTE3
#define EEPROM_CS_HIGH()   GPIO_PinSet(SPI_EEPROM_CS)
#define EEPROM_CS_LOW()    GPIO_PinClear(SPI_EEPROM_CS)
///=============================================================================================
typedef enum
{
	EEPROM_IDLE,
	EEPROM_INSTRUCTION,
	EEPROM_ADDRESS,
	EEPROM_DATA,
}EEPROM_STATE;
typedef struct 
{
	EEPROM_STATE nEepromState;
	SCH_U8 RD_State;
	SCH_U8 WR_State;
	SCH_U8 CP_State;///copy
}Eeprom_T;
extern Eeprom_T App_Eeprom;
#define CP_STATE   App_Eeprom.CP_State
#define WR_STATE   App_Eeprom.WR_State
#define RD_STATE   App_Eeprom.RD_State

extern void Eprom_IO_Init(void);
extern void EepromDataRxPro(void);
extern void TASK_Eeprom_Pro(void);
extern void Flash_RD_State_Clear(void);
extern void Flash_WR_State_Clear(void);
extern void Flash_CP_State_Clear(void);
extern SCH_BOOL Flash_Dsp_RD(SCH_U8 Num, void *data, SCH_U16 Cnt);
extern SCH_BOOL Flash_Dsp_WR(SCH_U8 Num, void *data, SCH_U16 Cnt);
extern SCH_BOOL Flash_Dsp_CP(SCH_U8 FromNum, SCH_U8 ToNum);
extern void Flash_Dsp_SE(SCH_U8 Num);
#endif
