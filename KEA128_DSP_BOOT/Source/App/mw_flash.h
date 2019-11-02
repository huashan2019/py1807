///=========================
#ifndef __MW_FLASH_H
#define __MW_FLASH_H

/* flash 128k
+--------------------------------------------------------------------+
| boot program                  | 16k| 0x000 0000   ---   0x000 3FFF |
|--------------------------------------------------------------------|
| app program                   |100k| 0x000 4000   ---   0x001 CFFF |
|--------------------------------------------------------------------|
| flash data                    | 11k| 0x001 D000   ---   0x001 FBFF |
|--------------------------------------------------------------------|
| iap flag                      |  1k| 0x001 FC00   ---   0x001 FFFF |
+--------------------------------------------------------------------+
*/



#define FLASH_PAGE_SIZE               0x200///512
#define FLASH_PAGE_CNT                5///The amount of data per page
#define FLASH_DATA_ADDR               0x001D000///Flash data basic addr


typedef enum
{
	FLASH_DATA_0,
	FLASH_DATA_1,
	FLASH_DATA_2,
	FLASH_DATA_3,	
	FLASH_DATA_MAX
}FLASH_DATA;

extern SCH_BOOL Flash_RD(FLASH_DATA flash_data, SCH_U32 *u32data);
extern SCH_BOOL Flash_RD_NUM(FLASH_DATA flash_data, SCH_U32 *u32data, SCH_U16 num);
extern SCH_BOOL Flash_WR(FLASH_DATA flash_data, SCH_U32 u32data);
extern SCH_BOOL Flash_WR_NUM(FLASH_DATA flash_data, SCH_U32 *u32data, SCH_U16 num);
extern SCH_BOOL Flash_Program_U8(SCH_U32 u32NVMTargetAddress,SCH_U8 *pData,SCH_U16 u16SizeBytes);
extern SCH_BOOL Flash_Program_U32(SCH_U32 u32NVMTargetAddress,SCH_U32 *pData,SCH_U16 u16SizeBytes);
extern void SysFlashInit(void);
#endif
