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
#include "iwdg.h"

uint8_t loraBuffer[20]; // LoRa�������ݻ�����
uint8_t menuID = '0';   // �û��˵�ID
Gpio_t Switch;          // ����̵���GPIO

void menu1();
void menu2();
void menu3();
void menu4();

void Init()
{
  BoardInitMcu();    // ��ʼ��MCU
  BoardInitPeriph(); // ��ʼ��Ƭ������
  keys_init();       // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  OLED_Init();       // ��ʼ��OLED
  OLED_Display_On(); // ��OLED

  NS_RadioInit(433000000, 10, 1000, 1000); // ��ʼ������ͨ��

  GpioInit(&Switch, PB_2, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0); // ��ʼ���̵���GPIO
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init(); // ��ʼ��
  while (1)
  {
    if (ReadRadioRxBuffer(loraBuffer)) // ������������
    {
      if (loraBuffer[0] != menuID) // ���ģʽ�Ƿ����仯
      {
        OLED_Clear();           // ģʽ�����仯,����
        menuID = loraBuffer[0]; // �л�ģʽID
      }

      switch (menuID) // ��ʾģʽ
      {
      case '0':
        menu1(); // ģʽ1
        break;
      case '1':
        menu2(); // ģʽ2
        break;
      case '2':
        menu3(); // ģʽ3
        break;
      case '3':
        menu4(); // ģʽ4
        break;
      }
    }
    DelayMs(100); // ϵͳ��ʱ(���������ض˱���һ��,�Ҳ���̫��)
  }
}

/**
 * @brief ģʽ1
 * ��ʾWIFI ͼ��
 */
void menu1()
{
  // OLED��ʾ
  OLED_ShowString(128 / 2 - 11 * 4, 0, "Select Menu");
  OLED_ShowCHinese(128 / 2 - 8, 4, 52);
}

/**
 * @brief ͬ�����ض���ʪ������
 * ������С����ֵʱ��LED1 and LED2
 */
void menu2()
{
  // OLED��ʾ
  OLED_ShowString(128 / 2 - 4 * 9, 0, "Auto Lamp");
  OLED_ShowString(0, 2, "Light:");
  OLED_ShowString(0, 4, "Temp:");
  OLED_ShowString(0, 6, "Humi:");

  // �������������е���ʪ������ղ�������ʾ��OLED��
  float light, temp, hum;
  strtok((char *)loraBuffer, ",");
  light = atof(strtok(NULL, ","));
  temp = atof(strtok(NULL, ","));
  hum = atof(strtok(NULL, ","));

  uint8_t lightData[6], tempData[5], humData[5];
  if (light < 10)
  {
    sprintf((char *)lightData, "%.2f  ", light);
  }
  else if (light < 100)
  {
    sprintf((char *)lightData, "%.2f ", light);
  }
  else if (light < 1000)
  {
    sprintf((char *)lightData, "%.2f", light);
  }

  sprintf((char *)tempData, "%.2f", temp);
  sprintf((char *)humData, "%.2f", hum);

  OLED_ShowString(7 * 8, 2, lightData);
  OLED_ShowString(6 * 8, 4, tempData);
  OLED_ShowString(6 * 8, 6, humData);

  // LED����
  if (light < 50)
  {
    GpioWrite(&Led1, 0);
    GpioWrite(&Led2, 0);
  }
  else
  {
    GpioWrite(&Led1, 1);
    GpioWrite(&Led2, 1);
  }
}

/**
 * @brief ģʽ3
 * ��Ե���ƽڵ�˼̵���
 */
void menu3()
{
  // ��ʾ����
  OLED_ShowString(128 / 2 - 9 * 4, 0, "Mula Lamp");

  // ������������
  strtok((char *)loraBuffer, ",");
  uint8_t cache = atol(strtok(NULL, ","));

  // �����������ݿ���LED1 and LED2 �� �̵���
  GpioWrite(&Switch, cache);
  GpioWrite(&Led1, !cache);
  GpioWrite(&Led2, !cache);

  // ��ʾ��Ӧ״̬
  if (cache)
    OLED_ShowString(0, 2, "Lamp:Open");
  else
    OLED_ShowString(0, 2, "Lamp:Close");
}

/**
 * @brief ������������
 * �����ض�ͬ����ʾ����ͨѶƵ��
 */
void menu4()
{
  OLED_ShowString(128 / 2 - 9 * 4, 0, "View Freq");
  OLED_ShowString(0, 2, "Freq:433000000");
}