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
  keys_init(); // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  OLED_Init();       //  ��ʼ��OLED
  OLED_Display_On(); // ����OLED��ʾ

  ADCS_Init(); // ��ʼ��ADC
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
    AdcScanChannel(); // ADCת��
    float light = AdcReadCh0() * 250.0;

    OLED_ShowNum(128 / 2, 3, light, 4, 16);

    if (light < 100)
      GpioWrite(&Led2, 0);
    else
      GpioWrite(&Led2, 1);
  }
}
