#ifndef __FLASH_H__
#define __FLASH_H__
#include "stdint.h"
#include "stm32l1xx_hal.h" 

#define FLASH_LG_FREQUENCY_ADDR            0x0800f000
#define FLASH_SENSOR_TYPE_ADDR      0x0800f100
#define FLASH_TRANS_DEV_ADDR		  0x0800f200

void WriteFlashTest(uint32_t Addr,uint32_t WriteFlashData);
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);   
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *PBuffer,uint32_t NumToRead);
uint32_t PrintFlashTest(uint32_t FLASH_ADDR);	
uint32_t STMFLASH_ReadWord(uint32_t faddr);
#endif

