/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : app_debug.c  ---  
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2018.10.19
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"



void Debug_CMD(SCH_U8 cmd)
{
	switch(cmd) 
	{
		case 0x00:
			break;
		case 0x01:///dsp初始化
			default_download_IC_1();
			break;
		case 0x02:///dsp复位
			DSP_RESET_HOLD;
			SysWaitMs(500);
			DSP_RESET_RELEASE;
			break;
		case 0x03:///ad1938复位
			AD1938_RESET_HOLD;
			SysWaitMs(500);
			AD1938_RESET_RELEASE;
			break;
		case 0x04:///ad1978复位
			AD1978_RESET_HOLD;
			SysWaitMs(500);
			AD1978_RESET_RELEASE;
			break;
		case 0x05:///BT复位
			BT_RESET_HOLD;
			SysWaitMs(500);
			BT_RESET_RELEASE;
			break;
		case 0x06:///ARM复位
			ARM_RESET_HOLD;
			SysWaitMs(500);
			ARM_RESET_RELEASE;
			break;
		default:break;
	}
}





















