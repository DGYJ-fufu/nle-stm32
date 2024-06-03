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

int keyId = 0, modId = 0;

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  OLED_Init();       // 初始化OLED
  OLED_Display_On(); // 开启OLED

  // 初始化呼吸灯
  setBreathLedArg(100, 50);
  startLedBreath();

  startLedBlink();
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();

  int index = 52;
  for (int i = 0; i < 4; i++)
  {
    OLED_ShowCHinese(16 * i, 0, index);
    index++;
  }
  for (int a = 0; a < 3; a++)
  {
    for (int i = 0; i < 4; i++)
    {
      OLED_ShowCHinese(16 * i + 16, 2 * a + 2, index);
      index++;
    }
  }

  OLED_ShowString(0, 2, "1.");
  OLED_ShowString(0, 4, "2.");
  OLED_ShowString(0, 6, "3.");

  OLED_ShowChar(16 * 5 + 8, 2 * keyId + 2, '<');
  while (1)
  {
    if (isKey2Pressed())
    {
      OLED_ShowChar(16 * 5 + 8, 2 * keyId + 2, '  ');
      keyId--;
      if (keyId < 0)
        keyId = 2;
      OLED_ShowChar(16 * 5 + 8, 2 * keyId + 2, '<');
      resetKey2();
    }

    if (isKey3Pressed())
    {
      OLED_ShowChar(16 * 5 + 8, 2 * keyId + 2, '  ');
      keyId++;
      if (keyId > 2)
        keyId = 0;
      OLED_ShowChar(16 * 5 + 8, 2 * keyId + 2, '<');
      resetKey3();
    }

    if (isKey4Pressed())
    {
      modId = keyId;
      resetKey4();
    }

    switch (modId)
    {
    case 0:
      GpioWrite(&Led1, 0);
      GpioWrite(&Led2, 0);
      break;
    case 1:
      breathLed();
      break;
    case 2:
      blinkPerHelfSecond();
      DelayMs(10);
      break;
    }
  }
}
