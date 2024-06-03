/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 */
#include <string.h>
#include "board.h"
#include "hal_key.h"
#include "tim-board.h"
#include "timer_handles.h"
#include "math.h"
#include "adc_reader.h"

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  OLED_Init();       //  初始化OLED
  OLED_Display_On(); // 开启OLED显示

  ADCS_Init(); // 初始化ADC
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  for (int i = 0; i < 3; i++)
  {
    OLED_ShowCHinese(128 / 2 - 16 * 3 - 8 + 16 * i, 3, 52 + i);
  }
  OLED_ShowChar(128 / 2 - 8, 3, ':');
  OLED_ShowString(128 / 2 + 8 * 4, 3, "Lx");

  while (1)
  {
    AdcScanChannel(); // ADC转换
    float light = AdcReadCh0() * 250.0;

    OLED_ShowNum(128 / 2, 3, light, 4, 16);

    if (light < 100)
      GpioWrite(&Led2, 0);
    else
      GpioWrite(&Led2, 1);
  }
}
