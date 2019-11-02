///=========================
#ifndef __APP_AT_H
#define __APP_AT_H
///===============================================
#define AT_START_END_S       0x0D
#define AT_START_END_E       0x0A
///=======================================================
#define BT_NAME_SET         "AT+NAME=DSP_DSD\r"
#define BT_NAME_GET         "AT+NAME?\r"
#define BT_FACTORYRESET     "AT+FACTORYRESET\r"
#define BT_LBDADDR_GET      "AT+LBDADDR?\r"
///======================================================
extern SCH_U8 BT_Addr[12];
extern void AtDataAnalyse(SCH_U8 *Data);
#endif
