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

uint32_t menuID = 0;   // �û��˵�ID
uint32_t keyID = 2;    // ѡ��˵�ID
uint8_t LEDStatus = 0; // ģʽ��LED��̵���״̬

void menu1();    // ģʽ1
void menu2();    // ģʽ2
void menu3();    // ģʽ3
void menu4();    // ģʽ4
void sendMode(); // ����ģʽ

/**
 * @brief ��Ƭ����ʼ��
 *
 */
void Init()
{
  BoardInitMcu();    // ��ʼ��MCU
  BoardInitPeriph(); // ��ʼ��Ƭ������
  keys_init();       // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  NS_RadioInit(433000000, 10, 1000, 1000); // ��ʼ������ͨ��

  ADCS_Init();      // ADC��ʼ��    (���ձ���)
  hal_temHumInit(); // ��ʼ����ʪ�� (��ʪ�ȱ���)

  OLED_Init();       // ��ʼ��OLED  (OLED����)
  OLED_Display_On(); // ��OLED  (OLED����)
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init(); // ��ʼ��
  while (1)
  {
    switch (menuID) // ģʽѡ��
    {
    case 0:
      menu1(); // ģʽ1
      break;
    case 1:
      menu2(); // ģʽ2
      break;
    case 2:
      menu3(); // ģʽ3
      break;
    case 3:
      menu4(); // ģʽ4
      break;
    }
    DelayMs(100); // ϵͳ��ʱ
  }
}

/**
 * @brief ģʽ1
 * ��ʾģʽ�˵�
 * �ṩģʽѡ��
 */
void menu1()
{
  // OLED��ʾ
  OLED_ShowString(128 / 2 - 4 * 4, 0, "Menu");
  OLED_ShowString(0, 2, "1.Auto Lamp ");
  OLED_ShowString(0, 4, "2.Mula Lamp ");
  OLED_ShowString(0, 6, "3.View Freq ");

  OLED_ShowChar(12 * 8, keyID, '<'); // ��ʾ���

  // �����������
  if (isKey2Pressed()) // �������
  {
    OLED_ShowChar(12 * 8, keyID, ' ');
    keyID -= 2;
    if (keyID <= 0)
    {
      keyID = 6;
    }
    resetKey2();
  }
  if (isKey3Pressed()) // �������
  {
    OLED_ShowChar(12 * 8, keyID, ' ');
    keyID += 2;
    if (keyID > 6)
    {
      keyID = 2;
    }
    resetKey3();
  }
  if (isKey4Pressed()) // ģʽѡ��
  {
    OLED_ShowChar(12 * 8, keyID, ' ');
    switch (keyID)
    {
    case 2:
      menuID = 1;
      break;
    case 4:
      menuID = 2;
      break;
    case 6:
      menuID = 3;
      break;
    default:
      break;
    }
    OLED_Clear(); // ����
    resetKey4();
  }
  sendMode(); // ����ģʽ����
}

/**
 * @brief ģʽ2
 * ��ʾʵʱ��ʪ�ȼ�����
 * ���յ�����ֵ����LED1 and LED2
 */
void menu2()
{
  // �������ݱ���
  float light, temp, hum;
  uint8_t tempData[5], humData[5], lightData[6];

  // ��������ֵ
  AdcScanChannel();                             // ת��ADC˫�ŵ��Ĵ�������Ϊ��ѹֵ
  light = ((5.0 / 2.0) * AdcReadCh0()) * 100.0; // ͨ����ʽ��ù���
  call_sht11(&temp, &hum);                      // ��ȡ��ʪ������

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

  // OLED��ʾ
  OLED_ShowString(128 / 2 - 4 * 9, 0, "Auto Lamp");
  OLED_ShowString(0, 2, "Light:");
  OLED_ShowString(0, 4, "Temp:");
  OLED_ShowString(0, 6, "Humi:");

  OLED_ShowString(6 * 8, 2, lightData);
  OLED_ShowString(5 * 8, 4, tempData);
  OLED_ShowString(5 * 8, 6, humData);

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

  // �˳�
  if (isKey4Pressed())
  {
    menuID = 0;
    OLED_Clear();
    resetKey4();
  }

  // ������������
  uint8_t loraBuffer[20];
  sprintf((char *)loraBuffer, "%d,%.2f,%.2f,%.2f", menuID, light, temp, hum);
  SX1276Send(loraBuffer, 20);
}

/**
 * @brief ģʽ3
 * ���ߵ�Ե���ƽڵ�˼̵���
 */
void menu3()
{

  // OLED��ʾ
  OLED_ShowString(128 / 2 - 9 * 4, 0, "Mula Lamp");
  if (LEDStatus)
  {
    OLED_ShowString(0, 2, "Lamp:Open");
  }
  else
  {
    OLED_ShowString(0, 2, "Lamp:Close");
  }

  // ����LED��ڵ�˼̵���
  if (isKey2Pressed())
  {
    GpioWrite(&Led1, 0);
    GpioWrite(&Led2, 0);
    LEDStatus = 1;
    resetKey2();
  }

  // �ر�LED��ڵ�˼̵���
  if (isKey3Pressed())
  {
    GpioWrite(&Led1, 1);
    GpioWrite(&Led2, 1);
    LEDStatus = 0;
    resetKey3();
  }

  // �˳�
  if (isKey4Pressed())
  {
    menuID = 0;
    OLED_Clear();
    resetKey4();
  }

  // ������������
  uint8_t loraBuffer[20];
  sprintf((char *)loraBuffer, "%d,%d", menuID, LEDStatus);
  SX1276Send(loraBuffer, 20);
}

/**
 * @brief ģʽ4
 * ��ʾ����ͨ��Ƶ��
 */
void menu4()
{
  // OLED��ʾ
  OLED_ShowString(128 / 2 - 9 * 4, 0, "View Freq");
  OLED_ShowString(0, 2, "Freq:433000000");
  OLED_ShowCHinese(0, 4, 52);

  // �˳�
  if (isKey4Pressed())
  {
    menuID = 0;
    OLED_Clear();
    resetKey4();
  }

  // ������������
  sendMode();
}

/**
 * @brief ������������
 * ��ڵ�˷���ģʽ����
 */
void sendMode()
{
  uint8_t loraBuffer[20];
  sprintf((char *)loraBuffer, "%d", menuID);
  SX1276Send(loraBuffer, 20);
}
