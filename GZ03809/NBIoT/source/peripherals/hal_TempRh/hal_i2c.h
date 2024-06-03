//=============================================================================
//    S E N S I R I O N
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  hal_i2c.h (V1.1)
// Brief     :  I2C hardware abstraction layer
//=============================================================================

#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

//-- Includes -----------------------------------------------------------------
#include "stm32l1xx_hal.h"         // controller register definitions

//-- Defines ------------------------------------------------------------------
//Processor endian system
//#define BIG ENDIAN   //e.g. Motorola (not tested at this time)
#define LITTLE_ENDIAN  //e.g. PIC, 8051, NEC V850
//=============================================================================
// basic types: making the size of types clear
//=============================================================================
typedef unsigned char   u8t;      ///< range: 0 .. 255
typedef signed char     i8t;      ///< range: -128 .. +127
                                      
typedef unsigned short  u16t;     ///< range: 0 .. 65535
typedef signed short    i16t;     ///< range: -32768 .. +32767
                                      
typedef unsigned long   u32t;     ///< range: 0 .. 4'294'967'295
typedef signed long     i32t;     ///< range: -2'147'483'648 .. +2'147'483'647
                                      
typedef float           ft;       ///< range: +-1.18E-38 .. +-3.39E+38
typedef double          dt;       ///< range:            .. +-1.79E+308

typedef enum{
  FALSE     = 0,
  TRUE      = 1
}bt;

typedef union {
  u16t u16;               // element specifier for accessing whole u16
  i16t i16;               // element specifier for accessing whole i16
  struct {
    #ifdef LITTLE_ENDIAN  // Byte-order is little endian
    u8t u8L;              // element specifier for accessing low u8
    u8t u8H;              // element specifier for accessing high u8
    #else                 // Byte-order is big endian
    u8t u8H;              // element specifier for accessing low u8
    u8t u8L;              // element specifier for accessing high u8
    #endif
  } s16;                  // element spec. for acc. struct with low or high u8
} nt16;

typedef union {
  u32t u32;               // element specifier for accessing whole u32
  i32t i32;               // element specifier for accessing whole i32
 struct {
    #ifdef LITTLE_ENDIAN  // Byte-order is little endian
    u16t u16L;            // element specifier for accessing low u16
    u16t u16H;            // element specifier for accessing high u16
    #else                 // Byte-order is big endian
    u16t u16H;            // element specifier for accessing low u16
    u16t u16L;            // element specifier for accessing high u16
    #endif
  } s32;                  // element spec. for acc. struct with low or high u16
} nt32;


//-- Enumerations -------------------------------------------------------------
// Error codes
typedef enum {
    NO_ERROR       = 0x00, // no error
    ACK_ERROR      = 0x01, // no acknowledgment error
    CHECKSUM_ERROR = 0x02, // checksum mismatch error
    TIMEOUT_ERROR  = 0x04, // timeout error
    PARM_ERROR     = 0x80, // parameter out of range error
} etError;

// I2C acknowledge
typedef enum {
    ACK  = 0,
    NACK = 1,
} etI2cAck;

//=============================================================================
void I2c_Init(void);// Initializes the ports for I2C interface.

//=============================================================================
void I2c_StartCondition(void);// Writes a start condition on I2C-Bus.

//=============================================================================
void I2c_StopCondition(void);// Writes a stop condition on I2C-Bus.

//=============================================================================
etError I2c_WriteByte(u8t txByte);// Writes a byte to I2C-Bus and checks acknowledge.

//=============================================================================
etError I2c_ReadByte(u8t *rxByte, etI2cAck ack, u8t timeout);//读1个字节

//=============================================================================
etError I2c_GeneralCallReset(void);//生成复位信号

//-----------------------------------------------------------------------------
void DelayMicroSeconds(u32t nbrOfUs);//延时nbrOfUs微秒

#endif
