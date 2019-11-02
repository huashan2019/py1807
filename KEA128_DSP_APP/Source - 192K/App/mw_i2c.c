/*
***************************************************************************************************
**  Copyright   : 
**  Project     : KEA128
**  File        : mw_i2c.c
**  Description : This file is the middleware.
**  Author      : WenjunHu
**  Created on  : 2017.08.08
**  Note        : NULL
***************************************************************************************************
*/
#include "include.h"

void I2CWait(void)
{
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
	SCH_NOP;SCH_NOP;SCH_NOP;
}
#define I2C0_WAIT      I2CWait()
#define I2C0_DELAY     SysWaitUs(10)
#define I2C1_WAIT      I2CWait()
#define I2C1_DELAY     SysWaitUs(10)
///=================================================================I2C0=======================================
void I2C0Init(void)
{
	I2C0_SCL_DDR_1;///
	I2C0_SCL_OUT_1;

	I2C0_SDA_DDR_1;
	I2C0_SDA_OUT_1;
}
void I2C0Start(void)
{
	I2C0_SCL_DDR_1;
	I2C0_SDA_DDR_1;
	I2C0_SDA_OUT_1;
	I2C0_SCL_OUT_1;
	I2C0_DELAY;
	I2C0_SDA_OUT_0;
	I2C0_DELAY;
 	I2C0_SCL_OUT_0;
	I2C0_DELAY;
}
void I2C0Stop(void)
{
	SCH_U8 i;
	I2C0_SCL_DDR_0;
	I2C0_SCL_OUT_1;
	for(i=0;(i<20)&&(!I2C0_SCL_IN);i++)//wait 200us
		I2C0_DELAY;
	I2C0_SCL_DDR_1;
	I2C0_SDA_DDR_1;
	I2C0_SDA_OUT_0;
	I2C0_WAIT;
	I2C0_SCL_OUT_1;
	I2C0_WAIT;
	I2C0_SDA_OUT_1;
	I2C0_WAIT;
}
SCH_U8 I2C0Out8Bit(SCH_U8 outbyte)
{
	SCH_U8 i;
	I2C0_SCL_DDR_1;
	I2C0_SDA_DDR_1;
	for(i=0x80;i>0;i>>=1)
	{
		I2C0_SCL_OUT_0;
		I2C0_WAIT;
		if(outbyte&i)
			I2C0_SDA_OUT_1;
		else
			I2C0_SDA_OUT_0;
		I2C0_WAIT;
		I2C0_SCL_OUT_1;
		I2C0_WAIT;
	}
	I2C0_SCL_OUT_0;
	I2C0_SDA_DDR_0;
	I2C0_WAIT;
	I2C0_SCL_OUT_1;
	I2C0_WAIT;
	if(I2C0_SDA_IN)
		i=NACK;
	else
		i=ACK;
	I2C0_WAIT;
	I2C0_SCL_OUT_0;
	I2C0_WAIT;
	return(i);
}
void I2C0Ack(void)
{
	I2C0_SCL_DDR_1;
	I2C0_SDA_DDR_1;
	I2C0_SDA_OUT_0;
	I2C0_WAIT;
	I2C0_SCL_OUT_1;
	I2C0_WAIT;
	I2C0_SCL_OUT_0;
	I2C0_SDA_DDR_0;
	I2C0_WAIT;
}
void I2C0NAck(void)
{
	I2C0_SCL_DDR_1;
	I2C0_SDA_DDR_1;
	I2C0_SDA_OUT_1;
	I2C0_WAIT;
	I2C0_SCL_OUT_1;
	I2C0_WAIT;
	I2C0_SCL_OUT_0;
	I2C0_SDA_DDR_0;
	I2C0_WAIT;
}
SCH_U8 I2C0In8Bit(void)
{
	SCH_U8 i,inbyte=0;
	I2C0_SCL_DDR_0;
	I2C0_SCL_OUT_1;
	for(i=0;(i<20)&&(!I2C0_SCL_IN);i++)
 		I2C0_DELAY;
	I2C0_SCL_DDR_1;
	I2C0_SDA_DDR_0;
	for(i=0x80;i>0;i>>=1)
	{
		I2C0_SCL_OUT_1;
		I2C0_WAIT;
		if(I2C0_SDA_IN)
			inbyte|=i;
		I2C0_WAIT;
		I2C0_SCL_OUT_0;
		I2C0_WAIT;
	}
	return (inbyte);
}
SCH_BOOL I2C0_Tx (SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb)
{
	SCH_U8 ii2c;
	I2C0Start();
	if(I2C0Out8Bit(dest_add)==NACK)
	{
		I2C0Stop();
		Printf("ADDR %x NACK \n", dest_add);
		return FALSE; 
	}
	for(ii2c=0; ii2c<nb; ii2c++)/*Loop to send all selected data from output buffer.*/
	{ 
		if(I2C0Out8Bit(*(buff+ii2c))==NACK) /* Next output buffer data byte sent.*/ 
		{
			I2C0Stop();
			Printf("ADDR %x NACK- \n", dest_add);
			return FALSE;  
		}
	}
	I2C0Stop();/* End of communication: stop condition generation.*/
	return TRUE;
}

SCH_BOOL I2C0_Tx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb)
{
	SCH_U8 ii2c;
	I2C0Start();
	if(I2C0Out8Bit(dest_add)==NACK)
	{
		I2C0Stop();
		Printf("ADDR %x NACK \n", dest_add);
		return FALSE; 
	}
	I2C0Out8Bit(sub_add);
	for(ii2c=0; ii2c<nb; ii2c++)/*Loop to send all selected data from output buffer.*/
	{ 
		if(I2C0Out8Bit(*(buff+ii2c))==NACK) /* Next output buffer data byte sent.*/ 
		{
			I2C0Stop();
			Printf("ADDR %x NACK- \n", dest_add);
			return FALSE;  
		}
	}
	I2C0Stop();/* End of communication: stop condition generation.*/
	return TRUE;
}

void I2C0_Rx (SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb)
{
	I2C0Start ();
	I2C0Out8Bit(dest_add+1);
	for(;nb>0;nb--,buff++)
	{
		*buff=I2C0In8Bit();
		if(nb==1)
			I2C0NAck();
		else
			I2C0Ack();
	}
	I2C0Stop(); 
}
void I2C0_Rx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb)
{
	I2C0Start ();
	I2C0Out8Bit(dest_add);
	I2C0Out8Bit(sub_add);
	
	I2C0Start();
	I2C0Out8Bit(dest_add+1);
	for(;nb>0;nb--,buff++)
	{
		*buff=I2C0In8Bit();
		if(nb==1)
			I2C0NAck();
		else
			I2C0Ack();
	}
	I2C0Stop(); 
}
SCH_BOOL I2C0_IsFindAddr(SCH_U8 Addr)
{
	SCH_U8 err=0;
	SCH_U8 correct=0;
	SCH_U8 count;
	for(count=0; count<3; count++) 
	{
		I2C0Start();
		if(I2C0Out8Bit(Addr)==NACK)
		{
			err++;
		}    
		else
		{
			correct++;
			I2C0Stop();      
		}
	}
	if(correct>=err)
		return TRUE;
	else 
		return FALSE;
}
////===========================================================================I2C1==========================================
void I2C1Init(void)
{
	I2C1_SCL_DDR_1;///
	I2C1_SCL_OUT_1;

	I2C1_SDA_DDR_1;
	I2C1_SDA_OUT_1;
}
void I2C1Start(void)
{
	I2C1_SCL_DDR_1;
	I2C1_SDA_DDR_1;
	I2C1_SDA_OUT_1;
	I2C1_SCL_OUT_1;
	I2C1_DELAY;
	I2C1_SDA_OUT_0;
	I2C1_DELAY;
 	I2C1_SCL_OUT_0;
	I2C1_DELAY;
}
void I2C1Stop(void)
{
	SCH_U8 i;
	I2C1_SCL_DDR_0;
	I2C1_SCL_OUT_1;
	for(i=0;(i<20)&&(!I2C1_SCL_IN);i++)//wait 200us
		I2C1_DELAY;
	I2C1_SCL_DDR_1;
	I2C1_SDA_DDR_1;
	I2C1_SDA_OUT_0;
	I2C1_WAIT;
	I2C1_SCL_OUT_1;
	I2C1_WAIT;
	I2C1_SDA_OUT_1;
	I2C1_WAIT;
}
SCH_U8 I2C1Out8Bit(SCH_U8 outbyte)
{
	SCH_U8 i;
	I2C1_SCL_DDR_1;
	I2C1_SDA_DDR_1;
	for(i=0x80;i>0;i>>=1)
	{
		I2C1_SCL_OUT_0;
		I2C1_WAIT;
		if(outbyte&i)
			I2C1_SDA_OUT_1;
		else
			I2C1_SDA_OUT_0;
		I2C1_WAIT;
		I2C1_SCL_OUT_1;
		I2C1_WAIT;
	}
	I2C1_SCL_OUT_0;
	I2C1_SDA_DDR_0;
	I2C1_WAIT;
	I2C1_SCL_OUT_1;
	I2C1_WAIT;
	if(I2C1_SDA_IN)
		i=NACK;
	else
		i=ACK;
	I2C1_WAIT;
	I2C1_SCL_OUT_0;
	I2C1_WAIT;
	return(i);
}
void I2C1Ack(void)
{
	I2C1_SCL_DDR_1;
	I2C1_SDA_DDR_1;
	I2C1_SDA_OUT_0;
	I2C1_WAIT;
	I2C1_SCL_OUT_1;
	I2C1_WAIT;
	I2C1_SCL_OUT_0;
	I2C1_SDA_DDR_0;
	I2C1_WAIT;
}
void I2C1NAck(void)
{
	I2C1_SCL_DDR_1;
	I2C1_SDA_DDR_1;
	I2C1_SDA_OUT_1;
	I2C1_WAIT;
	I2C1_SCL_OUT_1;
	I2C1_WAIT;
	I2C1_SCL_OUT_0;
	I2C1_SDA_DDR_0;
	I2C1_WAIT;
}
SCH_U8 I2C1In8Bit(void)
{
	SCH_U8 i,inbyte=0;
	I2C1_SCL_DDR_0;
	I2C1_SCL_OUT_1;
	for(i=0;(i<20)&&(!I2C1_SCL_IN);i++)
 		I2C1_DELAY;
	I2C1_SCL_DDR_1;
	I2C1_SDA_DDR_0;
	for(i=0x80;i>0;i>>=1)
	{
		I2C1_SCL_OUT_1;
		I2C1_WAIT;
		if(I2C1_SDA_IN)
			inbyte|=i;
		I2C1_WAIT;
		I2C1_SCL_OUT_0;
		I2C1_WAIT;
	}
	return (inbyte);
}
SCH_BOOL I2C1_Tx(SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb)
{
	SCH_U8 ii2c;
	I2C1Start();
	if(I2C1Out8Bit(dest_add)==NACK)
	{
		I2C1Stop();
		Printf("ADDR %x NACK \n", dest_add);
		return FALSE; 
	}
	for(ii2c=0; ii2c<nb; ii2c++)/*Loop to send all selected data from output buffer.*/
	{ 
		if(I2C1Out8Bit(*(buff+ii2c))==NACK) /* Next output buffer data byte sent.*/ 
		{
			I2C1Stop();
			Printf("ADDR %x NACK- \n", dest_add);
			return FALSE;  
		}
	}
	I2C1Stop();/* End of communication: stop condition generation.*/
	return TRUE;
}

SCH_BOOL I2C1_Tx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb)
{
	SCH_U8 ii2c;
	I2C1Start();
	if(I2C1Out8Bit(dest_add)==NACK)
	{
		I2C1Stop();
		Printf("ADDR %x NACK \n", dest_add);
		return FALSE; 
	}
	I2C1Out8Bit(sub_add);
	for(ii2c=0; ii2c<nb; ii2c++)/*Loop to send all selected data from output buffer.*/
	{ 
		if(I2C1Out8Bit(*(buff+ii2c))==NACK) /* Next output buffer data byte sent.*/ 
		{
			I2C1Stop();
			Printf("ADDR %x NACK- \n", dest_add);
			return FALSE;  
		}
	}
	I2C1Stop();/* End of communication: stop condition generation.*/
	return TRUE;
}

void I2C1_Rx(SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb)
{
	I2C1Start ();
	I2C1Out8Bit(dest_add+1);
	for(;nb>0;nb--,buff++)
	{
		*buff=I2C1In8Bit();
		if(nb==1)
			I2C1NAck();
		else
			I2C1Ack();
	}
	I2C1Stop(); 
}

void I2C1_Rx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb)
{
	I2C1Start ();
	I2C1Out8Bit(dest_add);
	I2C1Out8Bit(sub_add);
	
	I2C1Start();
	I2C1Out8Bit(dest_add+1);
	for(;nb>0;nb--,buff++)
	{
		*buff=I2C1In8Bit();
		if(nb==1)
			I2C1NAck();
		else
			I2C1Ack();
	}
	I2C1Stop(); 
}

SCH_BOOL I2C1_IsFindAddr(SCH_U8 Addr)
{
	SCH_U8 err=0;
	SCH_U8 correct=0;
	SCH_U8 count;
	for(count=0; count<3; count++) 
	{
		I2C1Start();
		if(I2C1Out8Bit(Addr)==NACK)
		{
			err++;
		}    
		else
		{
			correct++;
			I2C1Stop();      
		}
	}
	if(correct>=err)
		return TRUE;
	else 
		return FALSE;
}


