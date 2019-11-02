#ifndef __SCH_CONFIG_H
#define __SCH_CONFIG_H
#include "common.h"
///====================================================================
typedef uint8_t     SCH_BOOL;	/* Bool */
typedef int8_t      SCH_S8;		/* signed char */
typedef uint8_t     SCH_U8;		/* unsigned char */
typedef int16_t     SCH_S16;		/* signed short */
typedef uint16_t    SCH_U16;		/* unsigned short */
typedef int32_t     SCH_S32;		/* signed int */
typedef uint32_t    SCH_U32;		/* unsigned int */
///typedef int64_t     SCH_S64;		/* signed int */
///typedef uint64_t SCH_U64;		/* unsigned int */

typedef SCH_U8 *pU8;

typedef void (*pfunction)(void); 
typedef void (*U8pfunction)(SCH_U8); 


///=============config=================
#include "mw_uart.h"
///#define PRINTF_EN
#define PRINTF_UART				SCH_Uart1
#define PRINTF_BAUDRATE         115200
#define SCH_TO_TXBUF(data)		UartTxData_Direct(PRINTF_UART, data, 1)///UartTxData(PRINTF_UART, data, 1)///
#ifdef PRINTF_EN
	#define Print_Init()        SysUartInit(PRINTF_UART,PRINTF_BAUDRATE)
#else
	#define Print_Init()
#endif
///==========
#define SCH_INT_DISABLE		DisableInterrupts
#define SCH_INT_ENABLE		EnableInterrupts

///==============================
#define SHC_NOP        __NOP()

///=================================================
#include "sch_queue.h"
#include "sch_common.h"
#include "sch_message.h"
#include "sch_printf.h"

#endif
