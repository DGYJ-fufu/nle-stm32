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
#include "led_light.h"

int LED1Status = 0, LED2Status = 0;

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  OLED_Init();       //  初始化OLED
  OLED_Display_On(); // 开启OLED

  setBreathLedArg(100, 50);
  switchLed2Breath();

  NS_RadioInit(433000000, 10, 1000, 1000); // 初始化无线
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  OLED_ShowString(128 / 2 - 16 * 2 - 8, 1, "LED1: ");
  OLED_ShowString(128 / 2 - 16 * 2 - 8, 5, "LED2: ");

  OLED_ShowCHinese(128 / 2 + 8, 1, 54);
  OLED_ShowCHinese(128 / 2 + 8 + 16, 1, 55);

  OLED_ShowCHinese(128 / 2 + 8, 5, 54);
  OLED_ShowCHinese(128 / 2 + 8 + 16, 5, 55);

  while (1)
  {
    if (LED2Status)
    {
      breathLed();
    }

    uint8_t loraBuffer[2];
    if (ReadRadioRxBuffer(loraBuffer))
    {
      if (loraBuffer[0] == '1')
      {
        int cache = atol(&loraBuffer[1]);
        if (cache != LED1Status)
        {
          LED1Status = cache;
          if (LED1Status)
          {
            OLED_ShowCHinese(128 / 2 + 8, 1, 52);
            OLED_ShowCHinese(128 / 2 + 8 + 16, 1, 53);
          }
          else
          {
            OLED_ShowCHinese(128 / 2 + 8, 1, 54);
            OLED_ShowCHinese(128 / 2 + 8 + 16, 1, 55);
          }
          GpioWrite(&Led1, !LED1Status);
        }
      }
      else if (loraBuffer[0] == '2')
      {
        uint32_t cache = atol(&loraBuffer[1]);
        if (cache != LED2Status)
        {
          LED2Status = cache;
          if (!LED2Status)
          {
            GpioWrite(&Led2, 1);
            OLED_ShowCHinese(128 / 2 + 8, 5, 54);
            OLED_ShowCHinese(128 / 2 + 8 + 16, 5, 55);
          }
          else
          {
            OLED_ShowCHinese(128 / 2 + 8, 5, 56);
            OLED_ShowCHinese(128 / 2 + 8 + 16, 5, 57);
          }
        }
      }
    }
  }
}
