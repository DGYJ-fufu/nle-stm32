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

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  // ADCS_Init();
  hal_temHumInit(); //  ��ʼ����ʪ��

  OLED_Init();       // ��ʼ��OLED
  OLED_Display_On(); // ����OLED

  USART1_Init(115200); // ��ʼ������1
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {
    float temp, hum;         // �������ݱ���
    call_sht11(&temp, &hum); // ��ȡ��ʪ��

    // ��ʾ����
    for (int i = 0; i < 5; i++)
    {
      OLED_ShowCHinese((128 / 2 - 16 * 2 - 8) + 16 * i, 0, 52 + i);
    }

    // ��ʾ�¶�
    OLED_ShowCHinese(0, 2, 57);
    OLED_ShowCHinese(17, 2, 58);
    OLED_ShowNum(16 * 2 + 8, 2, (int)temp, 2, 16);
    OLED_ShowCHinese(16 * 3 + 8, 2, 60);

    // ��ʾʪ��
    OLED_ShowCHinese(0, 4, 59);
    OLED_ShowCHinese(17, 4, 58);
    OLED_ShowNum(16 * 2 + 8, 4, (int)hum, 2, 16);
    OLED_ShowChar(16 * 3 + 8, 4, '%');

    uint8_t data[100]; // �������ݻ�����

    // �����¶ȿ���LED2��ͨ�����ڷ�������
    if (temp < 28)
    {
      GpioWrite(&Led2, 0);
      sprintf((char *)data, "Temperature=%.6f��, Humidity=%.6f%%\nTemperature=%d��, Humidity=%d%%\nLED״̬:����", temp, hum, (int)temp, (int)hum);
    }
    else
    {
      GpioWrite(&Led2, 1);
      sprintf((char *)data, "Temperature=%.6f��, Humidity=%.6f%%\nTemperature=%d��, Humidity=%d%%\nLED״̬:�ر�", temp, hum, (int)temp, (int)hum);
    }
    USART1_SendStr(&data, 90); // ���ڷ�������

    Delay(1); // ��ʱ1s
  }
}
