/* Includes ------------------------------------------------------------------*/
#include "misc.h"

/*******************************************************************
*函数：uint8_t CHK(uint8_t *buf, uint8_t len)
*功能：计算累加和
*输入：uint8_t *buf ：需要累加的数
			 uint8_t len ：累加数的个数
*返回：累加和
*特殊说明：
*******************************************************************/
uint8_t CHK(uint8_t *buf, uint8_t len) //校验码
{
    uint8_t  RX_CHX=0;

    while(len--)
    {
        RX_CHX+= *buf;
        buf++;
    }

    return RX_CHX&=0xff;
}

uint32_t getCRC(uint8_t *buf, uint8_t len) {
    uint32_t crc = 0x0000ffff;
    for (int i = 0; i < len; i++) {
        crc ^= ((int) buf[i] & 0x000000ff);
        for (int j = 0; j < 8; j++) {
            if ((crc & 0x00000001) != 0) {
                crc >>= 1;
                crc ^= 0x0000a001;
            } else {
                crc >>= 1;
            }
        }
    }
		
    //高低位互换，输出符合相关工具对Modbus CRC16的运算
    return ((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8);
}

/*******************************************************************
*函数：void Uint16ToByteArray_BigEnd(uint16_t value, uint8_t *ret)
*功能：byte数组转uint32整型
*输入：uint8_t *buf ：二字节byte数组
*返回：uint16_t型整
*特殊说明：
*******************************************************************/
void Uint16ToByteArray_BigEnd(uint16_t value, uint8_t *ret)
{
    ret[0]=(value)>>8;
    ret[1]=(value)&0x00ff;
}

/*******************************************************************
*函数：uint16_t ByteArray2Uint16_BigEnd(uint8_t *buf)
*功能：byte数组转uint32整型
*输入：uint8_t *buf ：二字节byte数组
*返回：uint16_t型整
*特殊说明：
*******************************************************************/
uint16_t ByteArray2Uint16_BigEnd(uint8_t *buf)
{
    uint32_t ret=0;

    ret=(0x00ff&buf[0])<<8;
    ret|=(0x00ff&buf[1]);

    return ret;
}

/*******************************************************************
*函数：uint32_t ByteArray2Uint32_BigEnd(uint8_t *buf)
*功能：byte数组转uint32整型
*输入：uint8_t *buf ：四字节byte数组
*返回：uint32_t型整
*特殊说明：
*******************************************************************/
uint32_t ByteArray2Uint32_BigEnd(uint8_t *buf)
{
    uint32_t ret=0;

    ret=(0x00ff&buf[0])<<24;
    ret|=(0x00ff&buf[1])<<16;
    ret|=(0x00ff&buf[2])<<8;
    ret|=(0x00ff&buf[3]);

    return ret;
}











