///=========================
#ifndef __APP_BT_H
#define __APP_BT_H
///=====================================================
#define HEAD_ADDRESS_BT      0xAA
#define HEAD_ADDRESS_MCU     0x55
///=====================================================
#define B2M_DSP_DATA     0x0A
#define M2B_DSP_DATA     0x0A

#define B2M_DSP_UPDATA     0x0B
#define M2B_DSP_UPDATA     0x0B

#define B2M_SPDIF_INFO     0x0C
#define M2B_SPDIF_INFO     0x0C
///======================================================
extern void BtDataAnalyse(void);
extern void TASK_Bt_Pro(void);
extern SCH_BOOL Is_PASSWORD(SCH_U8 *Data);
extern SCH_BOOL Set_PASSWORD(SCH_U8 *Data);
#endif
