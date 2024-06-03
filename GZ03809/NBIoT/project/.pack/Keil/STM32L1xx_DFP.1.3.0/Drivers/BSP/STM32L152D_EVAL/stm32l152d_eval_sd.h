/**
  ******************************************************************************
  * @file    stm32l152d_eval_sd.c
  * @author  MCD Application Team
  * @brief   This file includes the uSD card driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L152D_EVAL_SD_H
#define __STM32L152D_EVAL_SD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32L152D_EVAL
  * @{
  */
    
/** @addtogroup STM32L152D_EVAL_SD
  * @{
  */    

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM32L152D_EVAL_SD_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief SD Card information structure 
  */
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef
   
/** 
  * @brief  SD status structure definition  
  */     
#define MSD_OK         0x00
#define MSD_ERROR      0x01

/** 
  * @brief  SD transfer state definition  
  */     
#define   SD_TRANSFER_OK                ((uint8_t)0x00)
#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)
#define   SD_TRANSFER_ERROR             ((uint8_t)0x02)

/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/  
 
/** @defgroup STM32152D_EVAL_SD_Exported_Constants Exported Constants
  * @{
  */ 
#define SD_DETECT_PIN                    GPIO_PIN_7
#define SD_DETECT_GPIO_PORT              GPIOC
#define __SD_DETECT_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define SD_DETECT_IRQn                   EXTI9_5_IRQn
   
#define SD_DATATIMEOUT           ((uint32_t)100000000)
    
#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)
   
/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __HAL_RCC_DMA2_CLK_ENABLE
#define SD_DMAx_Tx_STREAM                 DMA2_Channel4  
#define SD_DMAx_Rx_STREAM                 DMA2_Channel4  
#define SD_DMAx_Tx_IRQn                   DMA2_Channel4_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Channel4_IRQn
#define SD_DMAx_Tx_IRQHandler             DMA2_Channel4_IRQHandler
#define SD_DMAx_Rx_IRQHandler             DMA2_Channel4_IRQHandler
#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7)

/**
  * @}
  */
  

/* Exported functions --------------------------------------------------------*/

/** @addtogroup STM32L152D_EVAL_SD_Exported_Functions
  * @{
  */
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ITConfig(void);
void    BSP_SD_DetectIT(void);
void    BSP_SD_DetectCallback(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
void    BSP_SD_IRQHandler(void);
void    BSP_SD_DMA_Tx_IRQHandler(void);
void    BSP_SD_DMA_Rx_IRQHandler(void);
uint8_t BSP_SD_GetCardState(void);
void    BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo);
uint8_t BSP_SD_IsDetected(void);

/* These __weak functions can be surcharged by application code in case the current settings
   (eg. interrupt priority, callbacks implementation) need to be changed for specific application needs */
void    BSP_SD_WriteCpltCallback(void);
void    BSP_SD_ReadCpltCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L152D_EVAL_SD_H */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
