/**********************************************************************************************************
*ʹ��˵����
*TIM2��TIM3�ӿں������û�Ҫʹ��TIM2��3�Ķ�ʱ���ж�ʱ���ȵ���Tim2McuInit()��Tim3McuInit(),�������������������
*�����ڣ���λ��MS��ȡֵ1~1023MS���û����ж�Ӧ�ó��������ӵ����ļ�ĩβ��HAL_TIM_PeriodElapsedCallback()�ĺ�����
*�״�����ʱ�����к�ÿ���ж�����ʱ�䵽�������ж��ڵ�Ӧ�ó��򣬲��Ұ��ж������ظ���
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
*������void Tim2McuInit(uint16_t PeriodValueMs)
*���ܣ�TIM2��ʱ����ʼ���������ж�����PeriodValueMs����
*���룺uint16_t PeriodValueMs�ж����ڣ���λ:ms��ȡֵ1~1023
*�������
����˵������ʱ����16λ��ʱ�������ʱ������1024ms.ԭ����64000��Ƶ����ʱ1ms������Ϊ64-1�����Զ�ʱ
*					16λ���ֵΪ65535���������65536=64*X�����X=1024
**********************************************************************************************/
void Tim2McuInit(uint16_t PeriodValueMs);

/**********************************************************************************************
*������void Tim3McuInit(uint16_t PeriodValueMs)
*���ܣ�TIM3��ʱ����ʼ���������ж�����PeriodValueMs����
*���룺uint16_t PeriodValueMs�ж����ڣ���λ:0.25ms��ȡֵ1~1023
*�������
*����˵������ʱ����16λ��ʱ�������ʱ������1024ms.ԭ����16000��Ƶ����ʱ0.25ms������Ϊ64-1�����Զ�ʱ
*					16λ���ֵΪ65535���������65536=64*X�����X=1024
**********************************************************************************************/
void Tim3McuInit(uint16_t PeriodValueMs);

void Tim4McuInit(uint16_t PeriodValueMs);

typedef void (* TimerCallback)(void);

void setTimer2Callback(TimerCallback timerCallback);

void setTimer3Callback(TimerCallback timerCallback);

void setTimer4Callback(TimerCallback timerCallback);
#endif /* _TIM_BOARD_H_ */

/********************************END OF FILE********************************/