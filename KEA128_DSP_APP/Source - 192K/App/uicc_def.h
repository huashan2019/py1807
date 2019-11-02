#ifndef _UICC_DEF_H
#define _UICC_DEF_H
typedef enum  
{
	NO_KEY,
	UICC_MUTE,			//静音
	UICC_PLAY_PAUSE,		//播放暂停
	UICC_SKIPF	,		//下一曲
	UICC_SKIPB	,			//上一曲
	UICC_EJECT	,				//出碟		
	UICC_SOURCE,		//源切换
	UICC_ANGLE_DEC,		//屏下翻
	UICC_ANGLE_INC,		//屏上翻
	UICC_PICTURE,		//打开亮度调节菜单
	UICC_DISP,				//显示切换	
	UICC_OPEN,		//翻屏
	UICC_AUDIO_SETUP,			//打开音频设置菜单
	UICC_TUNER,//UICC_AMFM,			//进入收音机
	UICC_ASPS,				//自动存台
	UICC_INFO,			//显示播放信息

	UICC_VOLUME_DOWN,		//音量减	
	UICC_VOLUME_UP,		//音量加
	UICC_UP	,	//上
	UICC_DOWN,	//下
	UICC_LEFT,		//左
	UICC_RIGHT,		//右
	UICC_POWER,		//电源按键
	UICC_0	,		//0
	UICC_1	,		//1
	UICC_2	,		//2
	UICC_3	,			//3
	UICC_4,	//4
	UICC_5,			//5
	UICC_6	,		//6	
	UICC_7	,			//7
	UICC_8,		//8
	UICC_9,		//9
	UICC_CLEAR=0x22	,		//清除
	UICC_SETUP	,		//设置
	UICC_STOP,		//停止
	UICC_SEARCH,		//查找
	UICC_ANGLE,		//角度
	UICC_TITLE,		
	UICC_EQ,///UICC_AUDIO,
	UICC_SUBTITLE,
	UICC_HOME,		//菜单//android 长按为home,短按为menu
	UICC_ZOOM,	
	UICC_FASTR,		//快退
	UICC_FASTF,	//快进
	UICC_SCAN,		//扫描
	UICC_RANDOM,///UICC_SHUFFLE,		//随机
	UICC_REPEAT,		//重复
	UICC_MARKAB,		//AB
	UICC_SLOWF,	
	UICC_SLOWB,
	UICC_SEL,			//
	UICC_ENTER=0x36,    	 //OK           
	UICC_1OR2,	
	UICC_SMART_CW=0x39	,	//右旋钮正旋
	UICC_SMART_CCW,		//右旋钮反旋
	UICC_DISPLAY=0x3d,//显示切换
	UICC_PLAY,		//播放
	UICC_PAUSE	,		//暂停
	UICC_FUNCTION,	//功能按键
	UICC_MENU,	//功能按键(android专用，Wince不要使用)  
	UICC_BACK,	//返回按键
	UICC_CH_UP=0x43,	//上一个频道
	UICC_CH_DOWN,	//下一个频道
	UICC_AUX,	//Aux
	UICC_APP,	//APP按键
	UICC_TIME,	//时间设置

	UICC_VOLUME_CW=0x50,		//音量旋钮正旋
	UICC_VOLUME_CCW,		//音量旋钮反旋

	UICC_ESC,		//退出
	UICC_BT,		//BT按键
	UICC_SD_CARD,		//进入SD模式
	UICC_TFT_STANDBY=0x5b	,		//进入或者退出Standby模式
	UICC_4S_FUN,			//进入IPOD模式
	UICC_SPEECH,//UICC_VOICE,
	UICC_BT_ACPTCALL,///UICC_DIAL,			//拨号	
	UICC_BT_HUNGUPCALL,////UICC_DONE,			//挂电?
/*用户自定义*/
	UICC_BMW_MODE=0x60,
	UICC_BMW_CD,
	UICC_BMW_RADIO,
	UICC_BMW_MENU,
	UICC_BMW_FMAM,
	UICC_BMW_ORIGINAL,
	UICC_BMW_ANDROID,

	UICC_POWER_ON,
   UICC_POWER_OFF,
   UICC_TFT_AWAKE,
   UICC_LOC_DX,
   UICC_STANDBY,
/****************************/
	UICC_SCREEN=0x77,
	UICC_COMPASS=0x78,
	UICC_MODE=0x79,///UICC_DVD=0x79,	//进入DVD模式
	UICC_NAVI=0x7B,	//进入导航界面
	UICC_MEDIA,
	UICC_SCREEN_OPEN=0x7E,
	UICC_SCREEN_CLOSE=0x7F,

	UICC_MAX=0xff
}UICC_KEY_CODE;







#endif
