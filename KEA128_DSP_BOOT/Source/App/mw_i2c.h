///=========================
#ifndef __MW_I2C_H
#define __MW_I2C_H

/**************************************************************************************
* FunctionName   : I2C0
**************************************************************************************/
#define IO_I2C0_SCL                  GPIO_PTA2///
#define IO_I2C0_SDA                  GPIO_PTA3///

#define I2C0_SCL_OUT_1               GPIOA->PSOR = (1<<(IO_I2C0_SCL%32)) 
#define I2C0_SCL_OUT_0               GPIOA->PCOR = (1<<(IO_I2C0_SCL%32)) 
#define I2C0_SCL_IN                  (GPIOA->PDIR & (1<<(IO_I2C0_SCL%32)))
#define I2C0_SCL_DDR_1               GPIO_PinInit(IO_I2C0_SCL, GPIO_PinOutput)
#define I2C0_SCL_DDR_0               GPIO_PinInit(IO_I2C0_SCL, GPIO_PinInput)

#define I2C0_SDA_OUT_1               GPIOA->PSOR = (1<<(IO_I2C0_SDA%32))///GPIO_PinSet(IO_I2C0_SDA)
#define I2C0_SDA_OUT_0               GPIOA->PCOR = (1<<(IO_I2C0_SDA%32))///GPIO_PinClear(IO_I2C0_SDA)
#define I2C0_SDA_IN                  (GPIOA->PDIR & (1<<(IO_I2C0_SDA%32)))///GPIO_ReadInputPin(IO_I2C0_SDA)
#define I2C0_SDA_DDR_1               GPIO_PinInit(IO_I2C0_SDA, GPIO_PinOutput)
#define I2C0_SDA_DDR_0               GPIO_PinInit(IO_I2C0_SDA, GPIO_PinInput)
/**************************************************************************************
* FunctionName   : I2C1
**************************************************************************************/
#define IO_I2C1_SCL                  GPIO_PTH4///
#define IO_I2C1_SDA                  GPIO_PTH3///

#define I2C1_SCL_OUT_1               GPIOB->PSOR = (1<<(IO_I2C1_SCL%32))
#define I2C1_SCL_OUT_0               GPIOB->PCOR = (1<<(IO_I2C1_SCL%32))
#define I2C1_SCL_IN                  (GPIOB->PDIR & (1<<(IO_I2C1_SCL%32)))
#define I2C1_SCL_DDR_1               GPIO_PinInit(IO_I2C1_SCL, GPIO_PinOutput)
#define I2C1_SCL_DDR_0               GPIO_PinInit(IO_I2C1_SCL, GPIO_PinInput)

#define I2C1_SDA_OUT_1               GPIOB->PSOR = (1<<(IO_I2C1_SDA%32))///GPIO_PinSet(IO_I2C1_SDA)
#define I2C1_SDA_OUT_0               GPIOB->PCOR = (1<<(IO_I2C1_SDA%32))///GPIO_PinClear(IO_I2C1_SDA)
#define I2C1_SDA_IN                  (GPIOB->PDIR & (1<<(IO_I2C1_SDA%32)))///GPIO_ReadInputPin(IO_I2C1_SDA)
#define I2C1_SDA_DDR_1               GPIO_PinInit(IO_I2C1_SDA, GPIO_PinOutput)
#define I2C1_SDA_DDR_0               GPIO_PinInit(IO_I2C1_SDA, GPIO_PinInput)


typedef enum{
   NACK=0,
   ACK
}ACK_STATE;

///=======================================================================================
extern void I2C0_Rx(SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb);
extern void I2C0_Rx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb);
extern SCH_BOOL I2C0_Tx(SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb);
extern SCH_BOOL I2C0_Tx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb);
extern void I2C0Init(void);
extern SCH_BOOL I2C0_IsFindAddr(SCH_U8 Addr);
///=======================================================================================
extern void I2C1_Rx(SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb);
extern void I2C1_Rx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb);
extern SCH_BOOL I2C1_Tx(SCH_U8 dest_add,SCH_U8 *buff,SCH_U8 nb);
extern SCH_BOOL I2C1_Tx_A (SCH_U8 dest_add,SCH_U8 sub_add,SCH_U8 *buff,SCH_U8 nb);
extern void I2C1Init(void);
extern SCH_BOOL I2C1_IsFindAddr(SCH_U8 Addr);

#endif
