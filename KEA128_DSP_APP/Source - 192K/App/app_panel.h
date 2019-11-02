///=========================
#ifndef __APP_PANEL_H
#define __APP_PANEL_H


#define AD_SW2_CHANNEL     	ADC_CHANNEL_AD1
#define AD_SW2_MAX_NUM           8




#define REMOTE					0
#define PANEL 					1
#define TOUCH_SCREEN 			2


/* KeyProperty
0:只有短按功能
1:有长按功能，但长按按键没有连续功能
2:有长按功能，并且长按按键有连续功能
*/
typedef struct
{
	SCH_U16 KeyADC_Data;
	const SCH_U16 KeyADC_DefaultData;
	const UICC_KEY_CODE ShortKeyCode;
	const UICC_KEY_CODE LongKeyCode;
	const SCH_U8 KeyProperty;
}KEY_AD_DATA;
extern KEY_AD_DATA AD_SW2_Tab[AD_SW2_MAX_NUM];
typedef enum
{
	RELEASE,
	HOLD
}R_L;
typedef enum
{
	NO_STUDY,
	STUDY_ING,
	STUDY_OK
}STUDY_STATE;
typedef enum
{
	Key_Up,
	Key_Down
}KEY_STATE;
typedef struct
{
	SCH_U8       KeyProperty  :2;
	STUDY_STATE  StudyState   :2;
	KEY_STATE    KeyStatus    :1;	
	R_L          F_HoldPress  :1;	
	SCH_U8       Reverse      :2;
}_PANEL_FLAG;
typedef union
{
	SCH_U8 byte;
	_PANEL_FLAG field;
}PANEL_FLAG;
typedef struct
{
	UICC_KEY_CODE ShortKeyCode;
	UICC_KEY_CODE LongKeyCode;///
	UICC_KEY_CODE BkKeyCode;
	UICC_KEY_CODE StudyKeyCode;
	PANEL_FLAG PanelFlag;
	SCH_U8 HoldTime;
	SCH_U8 StudyNum;
}KEY_INFO;
extern KEY_INFO AD_SW2_Info;


extern void ClrADC_Data(KEY_AD_DATA *pTable,SCH_U8 Max_Cnt);
extern void SetADC_Data(KEY_AD_DATA *pTable,SCH_U8 Max_Cnt);
extern SCH_BOOL GetKeyNum(UICC_KEY_CODE KeyCode, SCH_U8 *Num, SCH_U8 Max_Cnt, KEY_AD_DATA *pTable);
extern void PostKeyCode(SCH_U8 KeyCode,SCH_U8 type);
extern void AD_SW2_Init(void);
extern void PanelInit(void);
extern void TASK_Key_Pro(void);

#endif
