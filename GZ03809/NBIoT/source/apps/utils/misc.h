/* Includes ------------------------------------------------------------------*/

#ifndef __MISC_H
#define __MISC_H

#include "stm32l1xx_hal.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
*函数：uint8_t CHK(uint8_t *buf, uint8_t len)
*功能：计算累加和
*输入：uint8_t *buf ：需要累加的数
			 uint8_t len ：累加数的个数
*返回：累加和
*特殊说明：
*******************************************************************/
uint8_t CHK(uint8_t *buf, uint8_t len);
	
uint32_t getCRC(uint8_t *buf, uint8_t len);

/*******************************************************************
*函数：uint16_t ByteArray2Uint16_BigEnd(uint8_t *buf)
*功能：byte数组转uint32整型
*输入：uint8_t *buf ：二字节byte数组
*返回：uint16_t型整
*特殊说明：
*******************************************************************/
uint16_t ByteArray2Uint16_BigEnd(uint8_t *buf);

/*******************************************************************
*函数：uint32_t ByteArray2Uint32_BigEnd(uint8_t *buf)
*功能：byte数组转uint32整型
*输入：uint8_t *buf ：四字节byte数组
*返回：uint32_t型整
*特殊说明：
*******************************************************************/
uint32_t ByteArray2Uint32_BigEnd(uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */


