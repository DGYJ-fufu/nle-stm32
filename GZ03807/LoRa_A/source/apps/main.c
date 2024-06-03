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

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  NS_RadioInit(425000000, 10, 1000, 1000); // ��ʼ����Ƶ
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {
    if (isKey2Pressed())
    {
      SX1276Send("s", 1);
      resetKey2();
    }
    if (isKey3Pressed())
    {
      SX1276Send("h", 1);
      resetKey2();
    }
    if (isKey4Pressed())
    {
      SX1276Send("t", 1);
      resetKey4();
    }
    DelayMs(10);
  }
}
