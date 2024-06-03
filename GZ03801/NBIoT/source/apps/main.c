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

uint32_t date[3]; // 定义日期数据

int menu = 0, keyID = 0; // 定义菜单ID以及功能选择ID

void mode1();    // 显示模式
void mode2();    // 选择编辑项目模式
void mode3();    // 修改模式
void saveData(); // 保存数据到Flash
void usart();    //  串口修改日期

void Init()
{
  BoardInitMcu();
  BoardInitPeriph();
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  OLED_Init();       // 初始化OLED
  OLED_Display_On(); //  开启OLED

  USART1_Init(115200); // 初始化串口
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();                             // 初始化
  STMFLASH_Read(0x0800E000, date, 3); // 读取Flash数据
  while (1)
  {  
    switch (menu) // 模式选择
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

    usart(); // 串口修改日期
  }
}

void mode1()
{
  // 显示Title
  for (int i = 0; i < 4; i++)
  {
    OLED_ShowCHinese(0 + 16 * i, 1, 52 + i);
  }
  OLED_ShowChar(16 * 4, 1, ':');

  // 显示年份
  OLED_ShowNum(0, 3, date[0], 4, 16);
  OLED_ShowCHinese(16 * 3, 3, 56);

  // 显示月份
  OLED_ShowNum(0, 5, date[1], 2, 16);
  OLED_ShowCHinese(16 * 1, 5, 57);

  // 显示天数
  OLED_ShowNum(16 * 2, 5, date[2], 2, 16);
  OLED_ShowCHinese(16 * 3, 5, 58);

  // 进入编辑项目选择模式
  if (isKey4Pressed())
  {
    menu = 1;
    OLED_Clear();
    resetKey4();
  }
}

void mode2()
{
  // 显示固定文字
  OLED_ShowCHinese(0, 1, 56);
  OLED_ShowChar(16, 1, ':');
  OLED_ShowCHinese(0, 3, 57);
  OLED_ShowChar(16, 3, ':');
  OLED_ShowCHinese(0, 5, 58);
  OLED_ShowChar(16, 5, ':');

  // 显示日期
  OLED_ShowNum(16 * 2, 1, date[0], 4, 16);
  OLED_ShowNum(16 * 3, 3, date[1], 2, 16);
  OLED_ShowNum(16 * 3, 5, date[2], 2, 16);

  // 光标
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
 * @brief 保存数据到Flash
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
 * @brief 串口修改日期
 * 并根据修改结果通过串口返回对应的数据
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
      uint8_t msg[3] = {0xfb, 0x00, 0xfe}; // 定义成功数据报文
      saveData();                          // 保存数据
      USART1_SendStr(msg, 3);              // 串口发送
    }
    else
    {
      uint8_t msg[3] = {0xfb, 0x01, 0xfe};
      USART1_SendStr(msg, 3);
    }
  }
}
