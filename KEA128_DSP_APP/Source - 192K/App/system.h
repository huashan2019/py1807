///=========================
#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef union
{
	SCH_U32 byte;
	struct 
	{
		SCH_U32 SleepMode       :1;
		SCH_U32 Update_Flag     :1;
		SCH_U32 SysMute         :1;
		SCH_U32 AccStatus       :1;///0 OFF 1 ON
		SCH_U32 RevStatus       :1;///0 OFF 1 ON
		SCH_U32 Revinfo         :1;///0 OFF 1 ON -- 倒车信号
		SCH_U32 IllStatus       :1;///0 OFF 1 ON
		SCH_U32 ParkStatus      :1;///0 OFF 1 ON
		SCH_U32 NaviGateStatus  :1;///0 OFF 1 ON
		SCH_U32 ARM_WORKGateStatus  :1;///0 OFF 1 ON
		SCH_U32 PhoneGateStatus :1;///0 OFF 1 ON
		SCH_U32 SysPowerStatus  :1;///0 OFF 1 ON
		SCH_U32 CCFLPowerStatus :1;///0 OFF 1 ON
		SCH_U32 MuteStatus      :1;///0 OFF 1 ON
		SCH_U32 VolErrFlag      :1;
		SCH_U32 OsStartOk       :1;
		SCH_U32 AppStartOk      :1;
		SCH_U32 AccWakeUp       :1;
		SCH_U32 AudioInfo       :1;///声音信号
		SCH_U32 AccHasFlag      :1;///带有acc标志
		SCH_U32 StartFlag       :1;///启动标志
		SCH_U32 ArmUpdateFlag   :1;
	}Flag;
}FLAG_T;
typedef struct 
{
	SCH_U8 SchedulerTimer;
	FLAG_T Status;
	SCH_U32 Hardware_Ver;
	SCH_U32 TimeCounter;
}SYS_T;
extern SYS_T Sys;

#define Set_SLEEP_Mode           Sys.Status.Flag.SleepMode = ON 
#define Clr_SLEEP_Mode           Sys.Status.Flag.SleepMode = OFF 
#define Get_SLEEP_Mode           Sys.Status.Flag.SleepMode

#define Set_SYS_MUTE             Sys.Status.Flag.SysMute = ON 
#define Clr_SYS_MUTE             Sys.Status.Flag.SysMute = OFF 
#define Get_SYS_MUTE             Sys.Status.Flag.SysMute

#define Set_ACC_Flag             Sys.Status.Flag.AccStatus = ON 
#define Clr_ACC_Flag             Sys.Status.Flag.AccStatus = OFF 
#define Get_ACC_Flag             Sys.Status.Flag.AccStatus

#define Set_ACC_Has             Sys.Status.Flag.AccHasFlag = ON 
#define Clr_ACC_Has             Sys.Status.Flag.AccHasFlag = OFF 
#define Get_ACC_Has             Sys.Status.Flag.AccHasFlag

#define Set_AUDIO_Flag             Sys.Status.Flag.AudioInfo = ON 
#define Clr_AUDIO_Flag             Sys.Status.Flag.AudioInfo = OFF 
#define Get_AUDIO_Flag             Sys.Status.Flag.AudioInfo

#define Set_START_Flag             Sys.Status.Flag.StartFlag = ON 
#define Clr_START_Flag             Sys.Status.Flag.StartFlag = OFF 
#define Get_START_Flag             Sys.Status.Flag.StartFlag


#define Set_IAP_Mode             Sys.Status.Flag.Update_Flag = ON 
#define Clr_IAP_Mode             Sys.Status.Flag.Update_Flag = OFF 
#define Get_IAP_Mode             Sys.Status.Flag.Update_Flag

#define Set_VolErr_Flag          Sys.Status.Flag.VolErrFlag = ERROR 
#define Clr_VolErr_Flag          Sys.Status.Flag.VolErrFlag = NORMAL 
#define Get_VolErr_Flag          Sys.Status.Flag.VolErrFlag

#define Set_OsStartOk            Sys.Status.Flag.OsStartOk = ON
#define Clr_OsStartOk            Sys.Status.Flag.OsStartOk = OFF
#define Get_OsStartOk            Sys.Status.Flag.OsStartOk

#define Set_AppStartOk           Sys.Status.Flag.AppStartOk = ON
#define Clr_AppStartOk           Sys.Status.Flag.AppStartOk = OFF
#define Get_AppStartOk           Sys.Status.Flag.AppStartOk

#define Set_SysPower_Flag        Sys.Status.Flag.SysPowerStatus = ON 
#define Clr_SysPower_Flag        Sys.Status.Flag.SysPowerStatus = OFF
#define Get_SysPower_Flag        Sys.Status.Flag.SysPowerStatus

#define Set_REV_Flag             Sys.Status.Flag.RevStatus = ON 
#define Clr_REV_Flag             Sys.Status.Flag.RevStatus = OFF
#define Get_REV_Flag             Sys.Status.Flag.RevStatus	

#define Set_REV_Info             Sys.Status.Flag.Revinfo = ON 
#define Clr_REV_Info             Sys.Status.Flag.Revinfo = OFF
#define Get_REV_Info             Sys.Status.Flag.Revinfo	

#define Set_ILL_Flag             Sys.Status.Flag.IllStatus = ON 
#define Clr_ILL_Flag             Sys.Status.Flag.IllStatus = OFF
#define Get_ILL_Flag             Sys.Status.Flag.IllStatus	

#define Set_PARK_Flag            Sys.Status.Flag.ParkStatus = ON 
#define Clr_PARK_Flag            Sys.Status.Flag.ParkStatus = OFF
#define Get_PARK_Flag            Sys.Status.Flag.ParkStatus

#define Set_NAVI_GATE_Flag       Sys.Status.Flag.NaviGateStatus = ON 
#define Clr_NAVI_GATE_Flag       Sys.Status.Flag.NaviGateStatus = OFF
#define Get_NAVI_GATE_Flag       Sys.Status.Flag.NaviGateStatus

#define Set_ARM_WORK_GATE_Flag       Sys.Status.Flag.ARM_WORKGateStatus = ON 
#define Clr_ARM_WORK_GATE_Flag       Sys.Status.Flag.ARM_WORKGateStatus = OFF
#define Get_ARM_WORK_GATE_Flag       Sys.Status.Flag.ARM_WORKGateStatus



#define Set_BT_GATE_Flag         Sys.Status.Flag.PhoneGateStatus = ON 
#define Clr_BT_GATE_Flag         Sys.Status.Flag.PhoneGateStatus = OFF
#define Get_BT_GATE_Flag         Sys.Status.Flag.PhoneGateStatus


#define Set_MUTE_Flag             Sys.Status.Flag.MuteStatus = ON 
#define Clr_MUTE_Flag             Sys.Status.Flag.MuteStatus = OFF
#define Get_MUTE_Flag             Sys.Status.Flag.MuteStatus

#define Set_AccWakeUP_Flag             Sys.Status.Flag.AccWakeUp = ON 
#define Clr_AccWakeUP_Flag             Sys.Status.Flag.AccWakeUp = OFF
#define Get_AccWakeUP_Flag             Sys.Status.Flag.AccWakeUp

#define Set_ArmUpdata_Flag             Sys.Status.Flag.ArmUpdateFlag = ON
#define Clr_ArmUpdata_Flag             Sys.Status.Flag.ArmUpdateFlag = OFF
#define Get_ArmUpdata_Flag             Sys.Status.Flag.ArmUpdateFlag



#define F_1ms_Set  SetBit(Sys.SchedulerTimer,0)
#define F_1ms_Clr  ClrBit(Sys.SchedulerTimer,0)
#define F_1ms_Val  GetBit(Sys.SchedulerTimer,0)

#define F_2ms_Set  SetBit(Sys.SchedulerTimer,1)
#define F_2ms_Clr  ClrBit(Sys.SchedulerTimer,1)
#define F_2ms_Val  GetBit(Sys.SchedulerTimer,1)

#define F_4ms_Set  SetBit(Sys.SchedulerTimer,2)
#define F_4ms_Clr  ClrBit(Sys.SchedulerTimer,2)
#define F_4ms_Val  GetBit(Sys.SchedulerTimer,2)

#define F_8ms_Set  SetBit(Sys.SchedulerTimer,3)
#define F_8ms_Clr  ClrBit(Sys.SchedulerTimer,3)
#define F_8ms_Val  GetBit(Sys.SchedulerTimer,3)

#define F_16ms_Set  SetBit(Sys.SchedulerTimer,4)
#define F_16ms_Clr  ClrBit(Sys.SchedulerTimer,4)
#define F_16ms_Val  GetBit(Sys.SchedulerTimer,4)

#define F_32ms_Set  SetBit(Sys.SchedulerTimer,5)
#define F_32ms_Clr  ClrBit(Sys.SchedulerTimer,5)
#define F_32ms_Val  GetBit(Sys.SchedulerTimer,5)

#define F_64ms_Set  SetBit(Sys.SchedulerTimer,6)
#define F_64ms_Clr  ClrBit(Sys.SchedulerTimer,6)
#define F_64ms_Val  GetBit(Sys.SchedulerTimer,6)

#define F_100ms_Set  SetBit(Sys.SchedulerTimer,7)
#define F_100ms_Clr  ClrBit(Sys.SchedulerTimer,7)
#define F_100ms_Val  GetBit(Sys.SchedulerTimer,7)





extern void SysDataInit(void);
#endif

