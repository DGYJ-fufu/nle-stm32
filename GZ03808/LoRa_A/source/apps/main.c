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
#include "adc_reader.h"
#include "math.h"

uint32_t LED1Status = 0, LED2Status = 0;

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  NS_RadioInit(433000000, 10, 1000, 1000); // 初始化无线

  ADCS_Init(); // 初始化ADC

  hal_temHumInit(); // 初始化温湿度

  OLED_Init();       //  初始化OLED
  OLED_Display_On(); // 开启OLED
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {

    float light, temp, hum;
    call_sht11(&temp, &hum);
    AdcScanChannel();
    light = ((5.0 / 2.0) * AdcReadCh0()) * 100.0;

    // 显示光照
    OLED_ShowCHinese(0, 0, 52);
    OLED_ShowCHinese(16, 0, 53);
    OLED_ShowChar(16 * 2, 0, ':');
    OLED_ShowNum(16 * 2 + 8, 0, light, 3, 16);
    OLED_ShowString(16 * 2 + 8 + 16 * 2, 0, "Lx");

    // 显示温度
    OLED_ShowCHinese(0, 2, 54);
    OLED_ShowCHinese(16, 2, 55);
    OLED_ShowChar(16 * 2, 2, ':');
    OLED_ShowNum(16 * 2 + 8, 2, (int)temp, 3, 16);
    OLED_ShowCHinese(16 * 2 + 8 + 16 * 2, 2, 58);

    // 显示湿度
    OLED_ShowCHinese(0, 4, 56);
    OLED_ShowCHinese(16, 4, 57);
    OLED_ShowChar(16 * 2, 4, ':');
    OLED_ShowNum(16 * 2 + 8, 4, (int)hum, 3, 16);
    OLED_ShowString(16 * 2 + 8 + 16 * 2, 4, "%Rh");

    uint8_t loraBuffer[2];
    if (isKey2Pressed())
    {
      LED1Status = !LED1Status;
      sprintf((char *)loraBuffer, "1%d", LED1Status);
      SX1276Send(loraBuffer, 2);
      resetKey2();
    }

    if (isKey3Pressed())
    {
      LED2Status = !LED2Status;
      sprintf((char *)loraBuffer, "2%d", LED2Status);
      SX1276Send(loraBuffer, 2);
      resetKey3();
    }
  }
}
