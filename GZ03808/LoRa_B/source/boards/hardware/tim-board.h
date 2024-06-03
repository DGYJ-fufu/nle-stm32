/**********************************************************************************************************
*使用说明：
*TIM2和TIM3接口函数，用户要使用TIM2或3的定时器中断时，先调用Tim2McuInit()或Tim3McuInit(),函数的输入参数就是中
*断周期，单位是MS，取值1~1023MS，用户的中断应用程序请添加到本文件末尾的HAL_TIM_PeriodElapsedCallback()的函数空
*白处。定时器运行后，每逢中断周期时间到便运行中断内的应用程序，并且按中断周期重复。
*
*
**********************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIM_BOARD_H_
#define _TIM_BOARD_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIM2_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()
#define TIM3_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()
#define TIM4_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()
/* Exported functions ------------------------------------------------------- */
/**********************************************************************************************
*函数：void Tim2McuInit(uint16_t PeriodValueMs)
*功能：TIM2定时器初始化，设置中断周期PeriodValueMs毫秒
*输入：uint16_t PeriodValueMs中断周期，单位:ms，取值1~1023
*输出：无
特殊说明：定时器是16位定时器，最大定时周期是1024ms.原理：64000分频，定时1ms，周期为64-1，所以定时
*					16位最大值为65535，最大周期65536=64*X，求解X=1024
**********************************************************************************************/
void Tim2McuInit(uint16_t PeriodValueMs);

/**********************************************************************************************
*函数：void Tim3McuInit(uint16_t PeriodValueMs)
*功能：TIM3定时器初始化，设置中断周期PeriodValueMs毫秒
*输入：uint16_t PeriodValueMs中断周期，单位:0.25ms，取值1~1023
*输出：无
*特殊说明：定时器是16位定时器，最大定时周期是1024ms.原理：16000分频，定时0.25ms，周期为64-1，所以定时
*					16位最大值为65535，最大周期65536=64*X，求解X=1024
**********************************************************************************************/
void Tim3McuInit(uint16_t PeriodValueMs);

void Tim4McuInit(uint16_t PeriodValueMs);

typedef void (* TimerCallback)(void);

void setTimer2Callback(TimerCallback timerCallback);

void setTimer3Callback(TimerCallback timerCallback);

void setTimer4Callback(TimerCallback timerCallback);


typedef void (* TimerHandleCallback)(TIM_HandleTypeDef *htim);

void setTimerBaseMspInitCallback(TimerHandleCallback callback);

void setTimerPeriodElapsedCallback(TimerHandleCallback callback);

#endif /* _TIM_BOARD_H_ */

/********************************END OF FILE********************************/
