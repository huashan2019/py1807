/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : APP_tdf8541.c  ---   ¹¦·Å
**  Description : 
**  Author      : WenjunHu
**  Created on  : 2017.04.27
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

/* amplifier init data table*/
SCH_U8 tbl_Amp_Init[5]=
{
	/*IDB1 
	D7------ 0: Disable clip detection below 10v,  1:Enable  clip detection below 10v
	D6------ 0: channel 3 clip information on DIAG pin,   1:channel 3 clip information on STB pin
	D5------ 0: channel 1 clip information on DIAG pin,   1:channel 1 clip information on STB pin
	D4------ 0: channel 4 clip information on DIAG pin,   1:channel 4 clip information on STB pin
	D3------ 0: channel 2 clip information on DIAG pin,   1:channel 2 clip information on STB pin
	D2------ 0: disable AC load detection,   1:enable AC Load detection
	D1------ 0: start_up diagnostic disable, 1:start_up diagnostic enable
	D0------ 0: amplifer start not enable,   1:amplifer will start */ 
	0x01,

	/*IDB2 
	D7 D6------ 00: clip level detect 2%,  01:clip level detect 5%
	10: clip level detect 10%, 01:disable
	D5------ 0: temperature information on DIAG,   1: no temperature information on DIAG
	D4------ 0: load fault information on DIAG,   1:no load fault information on DIAG
	D3------ null
	D2------ 0: channel 1 and 3 not soft mute,   1:channel 1 and 3 soft mute
	D1------ 0: channel 2 and 4 not soft mute,   1:channel 2and 4 soft mute
	D0------ 0: all amplifer channel not fast mute,   1:fast mute all channel
	*/
	0x41,///0x40,

	/*IDB3
	D7------ null
	D6------ 0: amplifer channel 1 and 3 gain select 26db,  1:amplifer channel 1 and 3 gain select 16db	  		
	D5------ 0: amplifer channel 2 and 4 gain select 26db,  1:amplifer channel 2 and 4 gain select 16db	
	D4------ 0: temperature warning level is 145c,   1:temperature warning level is 122c
	D3------ 0: channel 3 enable,  1:channel 3 disable
	D2------ 0: channel 1 enable,  1:channel 1 disable
	D1------ 0: channel 4 enable,  1:channel 4 disable
	D0------ 0: channel 2 enable,  1:channel 2 disable*/
	0x00,

	/*IDB4
	D7------ 0:SVR capacitor used to filter the common mode voltage,   1:for usage in combination with DC-DC converater
	D6------ 0: active fast mute during shut down via STB,  1:active slow mute during shut down via STB  		
	D5------ 0: 16v over voltage warning not on DIAG pin,  1:16v over voltage warning  on DIAG pin
	D4------ 0: DC load inform on DBX,   1:AC load inform on DBX
	D3------ 
	D2------ 0: line driver mode ,  1:low gain mode
	D1------ 0: AC load detection low measurement,  1:AC load detection high measurement
	D0------ 0: Low Vp Mute set to 5.9V,  1:Low Vp Mute set to 7.5V*/
	0x01,

	/*IDB5
	D7------ 0: High efficiency diable,   1:High efficiency enable
	D6------ 0: 4 channels high efficiency mode  1:2*2channel high efficiency mode  		
	D5------ 
	D4------ 0: high efficient switch level set to 2 ohm load,   1:high efficient switch level set to 4 ohm load
	D3------ 
	D2------ 
	D1------ 
	D0------ */
	0x90,
};
/*-----------------------------------------------------------------------
Function name:	AmpPort_Init
Input:			N/A
Output:			N/A
Description:	init Amp Diagnose information indicate port
------------------------------------------------------------------------*/
void AmpPort_Init(void)
{
	TurnOn_AMP;
}
void TDF8541Init(void)
{	
	I2C1_Tx(App_Amp.AmpAddr, tbl_Amp_Init, sizeof(tbl_Amp_Init));
	I2C1_Tx(App_Amp.AmpAddr1, tbl_Amp_Init, sizeof(tbl_Amp_Init));
}
/*-----------------------------------------------------------------------
Function name:	AmpMute
Input:			OffOn:AUDIO_ON /AUDIO_OFF
Output:			N/A
Description:	mute on/off
------------------------------------------------------------------------*/
void TDF8541_AmpMute(SCH_BOOL OnOff)
{
	if(OnOff == ON)/*mute on*/
	{
		tbl_Amp_Init[1]|=0x01;
	}
	else/*mute off*/
	{
		tbl_Amp_Init[1]&=~0x01; 
	}
	I2C1_Tx(App_Amp.AmpAddr, tbl_Amp_Init, sizeof(tbl_Amp_Init));
	I2C1_Tx(App_Amp.AmpAddr1, tbl_Amp_Init, sizeof(tbl_Amp_Init));
}
void Amp8541_DLL_Loader(void)
{
	Printf("AmpType --- TDF8541 \n");
	App_Amp.pAmpInit = TDF8541Init;
	App_Amp.pAmpMute = TDF8541_AmpMute;
}






