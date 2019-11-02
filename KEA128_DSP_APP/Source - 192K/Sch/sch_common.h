#ifndef __SCH_COMMON_H
#define __SCH_COMMON_H


#define SCH_NULL  (void *) 0


#define NONE   0  

#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)

#ifdef  ON
#undef  ON
#endif
#define ON      (1)

#ifdef  OFF
#undef  OFF
#endif
#define OFF     (0)


#define ERROR       1
#define NORMAL      0

#define HIGH_LEVEL  1
#define LOW_LEVEL   0

#define DOWN		0
#define UP			1

#define SCH_ENABLE    1
#define SCH_DISABLE   0

#define LSB(WORD)     (SCH_U8)WORD
#define MSB(WORD)     (SCH_U8)(WORD>>8)
#define SCH_WORD(MSB,LSB)	  (SCH_U16)(((SCH_U16) MSB << 8) + LSB)

#define dim(x)         (sizeof(x)/sizeof(x[0]))

#define SetBit(VAR,Place)         ( (VAR) |= (SCH_U8)((SCH_U8)1<<(SCH_U8)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (SCH_U8)((SCH_U8)((SCH_U8)1<<(SCH_U8)(Place))^(SCH_U8)255) )
#define GetBit(Var,Place)         ( (Var& (1<<(Place&0x7)))==0?0:1)

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus, BitAction;



#define MCU32bit 1

#if(MCU32bit)
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short int U16;
typedef signed short int S16;
typedef unsigned int U32;
typedef signed int S32;
typedef unsigned long long int U64;
typedef signed long long int S64;
#endif

#define BYTE		u8
#define WORD	u16
 #define DWORD		u32
 

#ifndef SUCCESS
#define SUCCESS 1
#endif
#ifndef FAIL
#define FAIL 0
#endif

#define PARA_ERROR     0xFE

typedef struct BitChar {
	SCH_U8 B0:1;
	SCH_U8 B1:1;
	SCH_U8 B2:1;
	SCH_U8 B3:1;
	SCH_U8 B4:1;
	SCH_U8 B5:1;
	SCH_U8 B6:1;
	SCH_U8 B7:1;
} SBitChar;

#define UNCHAR		unsigned char
#define UCHAR		unsigned char
#define UNINT		unsigned int
#define UCHARCONST	unsigned char const
#define ULONG		unsigned long
#define UINT		unsigned int

typedef struct Bit_Char{
	UCHAR B0:1;
	UCHAR B1:1;
	UCHAR B2:1;
	UCHAR B3:1;
	UCHAR B4:1;
	UCHAR B5:1;
	UCHAR B6:1;
	UCHAR B7:1;
}BitChar;

typedef union  CharField{
	SCH_U8    byte;
	SBitChar  field;
} UCharField;

typedef struct Bit_Int {
	unsigned int b0:1;
	unsigned int b1:1;
	unsigned int b2:1;
	unsigned int b3:1;
	unsigned int b4:1;
	unsigned int b5:1;
	unsigned int b6:1;
	unsigned int b7:1;
	unsigned int b8:1;
	unsigned int b9:1;
	unsigned int b10:1;
	unsigned int b11:1;
	unsigned int b12:1;
	unsigned int b13:1;
	unsigned int b14:1;
	unsigned int b15:1;
} BitInt;

typedef union word_Union {
	unsigned int	data_Int;
	BitInt	data_Intbit;
} word_field;

typedef union  byte_format{
	unsigned char	data_byte;
	BitChar	data_Bbit;
} byte_field;

typedef union io_word 
{
	unsigned int	io_w;
	unsigned char	io_b[2];
}word_io;

typedef struct {
	unsigned int b0:1;
	unsigned int b1:1;
	unsigned int b2:1;
	unsigned int b3:1;
	unsigned int b4:1;
	unsigned int b5:1;
	unsigned int b6:1;
	unsigned int b7:1;
	unsigned int b8:1;
	unsigned int b9:1;
	unsigned int b10:1;
	unsigned int b11:1;
	unsigned int b12:1;
	unsigned int b13:1;
	unsigned int b14:1;
	unsigned int b15:1;
	unsigned int b16:1;
	unsigned int b17:1;
	unsigned int b18:1;
	unsigned int b19:1;
	unsigned int b20:1;
	unsigned int b21:1;
	unsigned int b22:1;
	unsigned int b23:1;
	unsigned int b24:1;
	unsigned int b25:1;
	unsigned int b26:1;
	unsigned int b27:1;
	unsigned int b28:1;
	unsigned int b29:1;
	unsigned int b30:1;
	unsigned int b31:1;
} Bit32;


typedef union
{
	U8 ByteData[2];
	U16 Data;
}U16ToU8;

typedef union
{
	S8 ByteData[2];
	S16 Data;
}S16ToS8;

typedef union
{
	S8 ByteData[4];
	S32 Data;
}S32ToS8;

typedef union
{
	U8 ByteData[4];
	U32 Data;
}U32ToU8;

#define bit0		(1<<0)
#define bit1		(1<<1)
#define bit2		(1<<2)
#define bit3		(1<<3)
#define bit4		(1<<4)
#define bit5		(1<<5)
#define bit6		(1<<6)
#define bit7		(1<<7)
#define bit8		(1<<8)
#define bit9		(1<<9)
#define bit10		(1<<10)
#define bit11		(1<<11)
#define bit12		(1<<12)
#define bit13		(1<<13)
#define bit14		(1<<14)
#define bit15		(1<<15)
#define bit16		(1<<16)
#define bit17		(1<<17)
#define bit18		(1<<18)
#define bit19		(1<<19)
#define bit20		(1<<20)
#define bit21		(1<<21)
#define bit22		(1<<22)
#define bit23		(1<<23)
#define bit24		(1<<24)
#define bit25		(1<<25)
#define bit26		(1<<26)
#define bit27		(1<<27)
#define bit28		(1<<28)
#define bit29		(1<<29)
#define bit30		(1<<30)
#define bit31		(1<<31)



extern void SysWait1Us(void);
extern void SysWaitUs(SCH_U32 Time);
extern void SysWaitMs(SCH_U32 Time);
extern SCH_U32 GetMax(SCH_U32 m,SCH_U32 n);
extern SCH_U32 GetMin(SCH_U32 m,SCH_U32 n);
extern SCH_U32 LimitMaxMin(SCH_U32 min,SCH_U32 Data,SCH_U32 max);
extern SCH_U8 LimitMaxMin_U8(SCH_U8 min,SCH_U8 Data,SCH_U8 max);
extern SCH_S8 LimitMaxMin_S8(SCH_S8 min,SCH_S8 Data,SCH_S8 max);
extern void sch_memset(void *s,SCH_U8 c,SCH_U16 n);
extern void sch_memcpy(void *to,  void const *from, SCH_U16 n);
extern SCH_U8 sch_compare(void const *COM_A, void const *COM_B, SCH_U16 n);
extern SCH_U8 GetCheckData_Add(SCH_U8 *data, SCH_U8 Len);
extern SCH_U8 GetCheckData_Xor(SCH_U8 *data, SCH_U8 Len);
extern SCH_BOOL IsInRange(SCH_U32 min,SCH_U32 data,SCH_U32 max);
extern SCH_BOOL IsInData(SCH_U32 data0,SCH_U32 data1,SCH_U32 step);
extern SCH_U16 GetAverage_U16(SCH_U16 *pTable, SCH_U8 Array_Cnt);
extern SCH_U8 GetAverage_U8(SCH_U8 *pTable, SCH_U8 Array_Cnt);
extern SCH_BOOL IsInArray_U8(SCH_U8 Data,SCH_U8 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num);
extern SCH_BOOL IsInArray_U16(SCH_U16 Data,SCH_U16 *pTable,SCH_U8 Array_Cnt,SCH_U8 *num);
extern void ExchangeTwoVariable_U8(SCH_U8 *Data0,SCH_U8 *Data1);
extern void ExchangeTwoVariable_U16(SCH_U16 *Data0,SCH_U16 *Data1);
extern void ExchangeTwoVariable_U32(SCH_U32 *Data0,SCH_U32 *Data1);
#endif
