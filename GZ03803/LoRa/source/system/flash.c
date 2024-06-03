#include "flash.h"
#include "stm32l1xx_hal_flash_ex.h"
#include "stm32l1xx_hal_flash.h"

/**********************************************************************************************
*函数：uint32_t STMFLASH_ReadWord(uint32_t faddr)
*功能：读取指定地址的字(32位数据)
*输入：faddr:读地址(此地址必须为2的倍数!!)
*输出：对应数据
*特殊说明：无
**********************************************************************************************/
uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
    return *(__IO uint32_t*)faddr;
}

/**********************************************************************************************
*函数：void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
*功能：读取EEPROM数组写入操作
*输入：WriteAddr:起始地址
			 pBuffer:数据指针
			 NumToWrite:字(32位)数
*输出：无
*特殊说明：无
**********************************************************************************************/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
{
    HAL_FLASH_Unlock();//锁住FLASH
    HAL_FLASHEx_DATAEEPROM_Unlock(); //解锁FLASH
    //擦除FLASH
    FLASH_EraseInitTypeDef FLASH_EraseTypeDef;//初始化FLASH_EraseInitTypeDef
    FLASH_EraseTypeDef.TypeErase = FLASH_TYPEERASE_PAGES;
    FLASH_EraseTypeDef.PageAddress = WriteAddr;
    FLASH_EraseTypeDef.NbPages = 1;
    uint32_t PageError = 0;//设置PageError
    HAL_FLASHEx_Erase(&FLASH_EraseTypeDef, &PageError);//调用擦除函数
    uint32_t i;
    for(i=0; i<NumToWrite; i++)
    {
        HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAM_WORD, WriteAddr, pBuffer[i]);//FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
        WriteAddr+=4;//地址增加4.
    }
    HAL_FLASHEx_DATAEEPROM_Lock();//锁住FLASH
    HAL_FLASH_Lock();//锁住FLASH
}

/**********************************************************************************************
*函数：void WriteFlashTest(uint32_t Addr,uint32_t WriteFlashData)
*功能：读取EEPROM数组写入操作
*输入：WriteAddr:起始地址
			 WriteFlashData:数据
*输出：无
*特殊说明：无
**********************************************************************************************/
void WriteFlashTest(uint32_t Addr,uint32_t WriteFlashData)
{
    HAL_FLASH_Unlock();//锁住FLASH
    HAL_FLASHEx_DATAEEPROM_Unlock();//解锁FLASH
    //擦除FLASH
    FLASH_EraseInitTypeDef FLASH_EraseTypeDef;//初始化FLASH_EraseInitTypeDef
    FLASH_EraseTypeDef.TypeErase = FLASH_TYPEERASE_PAGES;
    FLASH_EraseTypeDef.PageAddress = Addr;
    FLASH_EraseTypeDef.NbPages = 1;
    uint32_t PageError = 0;//设置PageError
    HAL_FLASHEx_Erase(&FLASH_EraseTypeDef, &PageError);//调用擦除函数
    HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAM_WORD, Addr, WriteFlashData);//对FLASH烧写
    HAL_FLASHEx_DATAEEPROM_Lock();//锁住FLASH
    HAL_FLASH_Lock();//锁住FLASH
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:字(32位)数
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *PBuffer,uint32_t NumToRead)
{
    uint32_t i;
    for(i=0; i<NumToRead; i++)
    {
        PBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
        ReadAddr+=4;//偏移4个字节.
    }
}

/**********************************************************************************************
*函数：uint32_t PrintFlashTest(uint32_t FLASH_ADDR)
*功能：读取指定地址的字(32位数据)
*输入：faddr:读地址(此地址必须为2的倍数!!)
*输出：对应数据
*特殊说明：无
**********************************************************************************************/
uint32_t PrintFlashTest(uint32_t FLASH_ADDR)
{
    uint32_t temp = *(__IO uint32_t*)(FLASH_ADDR);
	
    return temp;
}


