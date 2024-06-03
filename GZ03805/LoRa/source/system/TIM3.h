#ifndef _TIM3_H
#define _TIM3_H
#include "board.h"
#include "usart2-board.h"
#include "usart1-board.h"


extern TIM_HandleTypeDef TIM3_Handler;      //定时器3PWM句柄 

void TIM3_Init(uint16_t arr,uint16_t psc);    //定时器初始化
#endif

