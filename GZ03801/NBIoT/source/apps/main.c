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

uint32_t date[3]; // ������������

int menu = 0, keyID = 0; // ����˵�ID�Լ�����ѡ��ID

void mode1();    // ��ʾģʽ
void mode2();    // ѡ��༭��Ŀģʽ
void mode3();    // �޸�ģʽ
void saveData(); // �������ݵ�Flash
void usart();    //  �����޸�����

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // ������ʼ��
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // ��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��

  OLED_Init();       // ��ʼ��OLED
  OLED_Display_On(); //  ����OLED

  USART1_Init(115200); // ��ʼ������
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();                             // ��ʼ��
  STMFLASH_Read(0x0800E000, date, 3); // ��ȡFlash����
  while (1)
  {  
    switch (menu) // ģʽѡ��
    {
    case 0:
      mode1();
      break;
    case 1:
      mode2();
      break;
    case 2:
      mode3();
      break;
    }

    usart(); // �����޸�����
  }
}

void mode1()
{
  // ��ʾTitle
  for (int i = 0; i < 4; i++)
  {
    OLED_ShowCHinese(0 + 16 * i, 1, 52 + i);
  }
  OLED_ShowChar(16 * 4, 1, ':');

  // ��ʾ���
  OLED_ShowNum(0, 3, date[0], 4, 16);
  OLED_ShowCHinese(16 * 3, 3, 56);

  // ��ʾ�·�
  OLED_ShowNum(0, 5, date[1], 2, 16);
  OLED_ShowCHinese(16 * 1, 5, 57);

  // ��ʾ����
  OLED_ShowNum(16 * 2, 5, date[2], 2, 16);
  OLED_ShowCHinese(16 * 3, 5, 58);

  // ����༭��Ŀѡ��ģʽ
  if (isKey4Pressed())
  {
    menu = 1;
    OLED_Clear();
    resetKey4();
  }
}

void mode2()
{
  // ��ʾ�̶�����
  OLED_ShowCHinese(0, 1, 56);
  OLED_ShowChar(16, 1, ':');
  OLED_ShowCHinese(0, 3, 57);
  OLED_ShowChar(16, 3, ':');
  OLED_ShowCHinese(0, 5, 58);
  OLED_ShowChar(16, 5, ':');

  // ��ʾ����
  OLED_ShowNum(16 * 2, 1, date[0], 4, 16);
  OLED_ShowNum(16 * 3, 3, date[1], 2, 16);
  OLED_ShowNum(16 * 3, 5, date[2], 2, 16);

  // ���
  OLED_ShowCHinese(16 * 6, keyID * 2 + 1, 59);
  if (isKey2Pressed())
  {
    OLED_ShowString(16 * 6, keyID * 2 + 1, "  ");
    keyID--;
    if (keyID < 0)
    {
      keyID = 2;
    }
    OLED_ShowCHinese(16 * 6, keyID * 2 + 1, 59);
    resetKey2();
  }
  if (isKey3Pressed())
  {
    OLED_ShowString(16 * 6, keyID * 2 + 1, "  ");
    keyID++;
    if (keyID > 2)
    {
      keyID = 0;
    }
    OLED_ShowCHinese(16 * 6, keyID * 2 + 1, 59);
    resetKey3();
  }
  if (isKey4Pressed())
  {
    menu = 2;
    resetKey4();
  }
}

void mode3()
{
  OLED_ShowChar(16 * 7, keyID * 2 + 1, '*');
  if (isKey2Pressed())
  {
    date[keyID] += 1;
    if (keyID == 0)
    {
      OLED_ShowNum(16 * 2, 1, date[0], 4, 16);
    }
    else
    {
      OLED_ShowNum(16 * 3, 1 + keyID * 2, date[keyID], 2, 16);
    }
    resetKey2();
  }
  if (isKey3Pressed())
  {
    date[keyID] -= 1;
    if (keyID == 0)
    {
      OLED_ShowNum(16 * 2, 1, date[0], 4, 16);
    }
    else
    {
      OLED_ShowNum(16 * 3, 1 + keyID * 2, date[keyID], 2, 16);
    }
    resetKey3();
  }
  if (isKey4Pressed())
  {
    OLED_ShowChar(16 * 7, keyID * 2 + 1, ' ');
    menu = 1;
    saveData();
    resetKey4();
  }
}

/**
 * @brief �������ݵ�Flash
 */
void saveData()
{
  for (int i = 0; i < 5; i++)
  {
    STMFLASH_Write(0x0800E000, date, 3);
    DelayMs(100);
  }
}

/**
 * @brief �����޸�����
 * �������޸Ľ��ͨ�����ڷ��ض�Ӧ������
 */
void usart()
{
  uint8_t data[5];
  if (USART1_ReadRxBuffer(data) != 0)
  {
    int status = 0;
    if (data[0] == 0xfb)
    {
      if (data[1] == 0x01 && data[4] == 0xfe)
      {
        date[0] = (int)data[2] * 100 + (int)data[3];
        status = 1;
      }
      else if (data[1] == 0x02 && data[3] == 0xfe)
      {
        date[1] = (int)data[2];
        status = 1;
      }
      else if (data[1] == 0x03 && data[3] == 0xfe)
      {
        date[2] = (int)data[2];
        status = 1;
      }
    }
    if (status)
    {
      uint8_t msg[3] = {0xfb, 0x00, 0xfe}; // ����ɹ����ݱ���
      saveData();                          // ��������
      USART1_SendStr(msg, 3);              // ���ڷ���
    }
    else
    {
      uint8_t msg[3] = {0xfb, 0x01, 0xfe};
      USART1_SendStr(msg, 3);
    }
  }
}
