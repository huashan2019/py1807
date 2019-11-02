///=========================
#ifndef __APP_AMP_H
#define __APP_AMP_H

typedef enum
{
	AMP_IDLE,	
	AMP_ENABLE,
	AMP_CHECK,
	AMP_NORMAL,
	AMP_DISABLE,
	AMP_CLOSE
}_AMP_S;

typedef enum  
{
	TDF8541_A_TYPE,
	TDF8541_B_TYPE,
	TDF8541_C_TYPE,
	TDF8541_D_TYPE,
	MAX_AMP_TYPE
}_AmpSysType_t;

typedef struct 
{
	_AMP_S AmpState;
	SCH_U8 AmpAddr;
	SCH_U8 AmpAddr1;
	_AmpSysType_t	AmpType;
	_AmpSysType_t	AmpType1;
	SCH_BOOL AmpTypeCheck;	
	void (*pAmpInit)(void);	
	void (*pAmpMute)(SCH_BOOL OnOff);
}AMP_T;
extern AMP_T App_Amp;

#define AMP_TURN_ON()     App_Amp.AmpState=AMP_ENABLE
#define AMP_TURN_OFF()    App_Amp.AmpState=AMP_DISABLE

extern void AmpMute(SCH_BOOL OnOff);
extern void TASK_Amp_Pro(void);
#endif
