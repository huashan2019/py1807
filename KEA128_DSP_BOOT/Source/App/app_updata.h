#ifndef __APP_UPDATA_H
#define __APP_UPDATA_H




typedef enum
{
	IAP_IDLE,	
	IAP_LENGTH_CHECKSUNM,
	IAP_DATA,
	IAP_END	
}IAP_STATE;

typedef struct 
{
	SCH_U8 IAP_Data[128];
	SCH_U16 IAP_Sn;
	SCH_U32 IAP_Len;
	SCH_U32 IAP_Checksum;
	SCH_U32 IAP_Checksum1;
	SCH_U32 IAP_ResendTime;
	SCH_U8 IAP_DataOk;
	IAP_STATE IapState;
}Iap_T;
extern Iap_T Iap;




extern void Updata_Init(void);
extern void BootloaderJumpToApp(void);
extern SCH_BOOL Is_IapMode(void);
extern SCH_BOOL Is_ResetMode(void);
extern void In_IapMode(void);
extern void In_ResetMode(void);
extern void Out_IapMode(void);
extern void MCU_Updata_pro(void);
extern void IAP_Pro(void);
#endif
