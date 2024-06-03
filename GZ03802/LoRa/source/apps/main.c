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

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  ADCS_Init();      // ��ʼ��ADC
  hal_temHumInit(); // ��ʼ����ʪ��

  OLED_Init();       //  ��ʼ��OLED
  OLED_Display_On(); // ����OLED��ʾ

  USART1_Init(115200); // ��ʼ������
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {
    // ��ʾ�̶�����
    for (int i = 0; i < 4; i++)
    {
      OLED_ShowCHinese(128 / 2 - 16 * 2 + i * 16, 0, 52 + i);
    }
    OLED_ShowCHinese(0, 2, 56);
    OLED_ShowCHinese(16, 2, 57);
    OLED_ShowCHinese(0, 4, 58);
    OLED_ShowCHinese(16, 4, 59);
    OLED_ShowCHinese(0, 6, 60);
    OLED_ShowCHinese(16, 6, 61);

    // ��ȡ����������
    float light, temp, hum;
    AdcScanChannel(); // ADCת��
    light = pow(10, ((1.78 - log10(33 / AdcReadCh0() - 10)) / 0.6));
    call_sht11(&temp, &hum);

    // ��ʾ��������
    OLED_ShowNum(16 * 2 + 8, 2, temp, 2, 16);
    OLED_ShowNum(16 * 2 + 8, 4, hum, 2, 16);
    OLED_ShowNum(16 * 2 + 8, 6, light, 4, 16);

    // ��ʾ��λ
    OLED_ShowCHinese(16 * 4, 2, 62);
    OLED_ShowChar(16 * 4, 4, '%');
    OLED_ShowString(16 * 5, 6, "lux");

    // ���ݹ���ֵ����LED2
    if (light < 100)
      GpioWrite(&Led2, 0);
    else
      GpioWrite(&Led2, 1);

    // ���ڷ��ʹ���������
    uint8_t data[50];
    memset1(data, '\0', 50);
    // uint8_t data[50] = "temperature:27|humidity:36|light:1210";    // ������Ϣ
    sprintf((char *)data, "temperature:%.0f|humidity:%.0f|light:%.0f", temp, hum, light);
    if (HAL_GPIO_ReadPin(KEY2_GPIO, KEY2_GPIO_PIN) == GPIO_PIN_RESET)
    {
      USART1_SendStr(data, 50);
    }
    else
    {
      uint8_t cache[100];
      memset1(cache, '\0', 100);
      for (int i = 0; data[i] != '\0'; i++)
      {
        // cache + i * 2 Ϊָ���ַƫ��,��Ϊת��hex����Ҫ����λ�õĿռ�,������Ҫͨ��ƫ����Ԥ���ռ�
        // ��ѭ����ʼi=0,��ôָ��λ�þͻ�Ϊ0,Ҳ����Ϊ�ʼ��λ��
        // ��i=1ʱ,��ʱָ��λ�þ�Ϊ2,�����еĵ�����λ��
        sprintf(cache + i * 2, "%02X", data[i]);
      }
      USART1_SendStr(cache, 100);
    }

    DelayMs(500);
  }
}
