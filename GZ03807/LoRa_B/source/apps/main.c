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

int mode = 0, keyID = 0;

void mode1();
void mode2();
void mode3();

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  NS_RadioInit(425000000, 10, 1000, 1000); // ��ʼ����Ƶ

  OLED_Init();
  OLED_Display_On();

  setBreathLedArg(100, 50);
  switchLed2Breath();

  startLedBlink();
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {
    uint8_t loraBuffer[1];
    if (ReadRadioRxBuffer(loraBuffer) != 0)
    {

      if (loraBuffer[0] == 's') // ��˸
      {
        keyID = 1;
      }
      else if (loraBuffer[0] == 'h') // ����
      {
        keyID = 2;
      }
      else if (loraBuffer[0] == 't') // ֹͣ
      {
        keyID = 3;
      }

      if (mode != keyID)
      {
        mode = keyID;
        OLED_Clear();
        switch (mode)
        {
        case 1:
          OLED_ShowString(128 / 2 - 20 - 8 * 3, 3, "LED");
          for (int i = 0; i < 4; i++)
          {
            OLED_ShowCHinese(128 / 2 - 20 + i * 16, 3, 52 + i);
          }
          break;
        case 2:
          OLED_ShowString(128 / 2 - 16 * 2 - 8, 3, "LED2");
          for (int i = 0; i < 3; i++)
          {
            OLED_ShowCHinese(128 / 2 - 8 + i * 16, 3, 56 + i);
          }
          break;
        }
      }
    }

    switch (mode)
    {
    case 1:
      mode1();
      break;
    case 2:
      mode2();
      break;
    case 3:
      mode3();
      break;
    }
  }
}

void mode1()
{
  blinkPerHelfSecond();
  DelayMs(10);
}

void mode2()
{
  breathLed();
}

void mode3()
{
  GpioWrite(&Led1, 1);
  GpioWrite(&Led2, 1);
}