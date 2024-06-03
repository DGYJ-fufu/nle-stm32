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

uint8_t loraBuffer[20]; // LoRa无线数据缓存区
uint8_t menuID = '0';   // 用户菜单ID
Gpio_t Switch;          // 定义继电器GPIO

void menu1();
void menu2();
void menu3();
void menu4();

void Init()
{
  BoardInitMcu();    // 初始化MCU
  BoardInitPeriph(); // 初始化片上外设
  keys_init();       // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  OLED_Init();       // 初始化OLED
  OLED_Display_On(); // 打开OLED

  NS_RadioInit(433000000, 10, 1000, 1000); // 初始化无线通信

  GpioInit(&Switch, PB_2, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0); // 初始化继电器GPIO
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init(); // 初始化
  while (1)
  {
    if (ReadRadioRxBuffer(loraBuffer)) // 接收无线数据
    {
      if (loraBuffer[0] != menuID) // 检测模式是否发生变化
      {
        OLED_Clear();           // 模式发生变化,清屏
        menuID = loraBuffer[0]; // 切换模式ID
      }

      switch (menuID) // 显示模式
      {
      case '0':
        menu1(); // 模式1
        break;
      case '1':
        menu2(); // 模式2
        break;
      case '2':
        menu3(); // 模式3
        break;
      case '3':
        menu4(); // 模式4
        break;
      }
    }
    DelayMs(100); // 系统延时(建议与主控端保持一致,且不宜太高)
  }
}

/**
 * @brief 模式1
 * 显示WIFI 图标
 */
void menu1()
{
  // OLED显示
  OLED_ShowString(128 / 2 - 11 * 4, 0, "Select Menu");
  OLED_ShowCHinese(128 / 2 - 8, 4, 52);
}

/**
 * @brief 同步主控端温湿度数据
 * 当光照小于阈值时打开LED1 and LED2
 */
void menu2()
{
  // OLED显示
  OLED_ShowString(128 / 2 - 4 * 9, 0, "Auto Lamp");
  OLED_ShowString(0, 2, "Light:");
  OLED_ShowString(0, 4, "Temp:");
  OLED_ShowString(0, 6, "Humi:");

  // 处理无线数据中的温湿度与光照并将其显示在OLED上
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

  // LED控制
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
 * @brief 模式3
 * 点对点控制节点端继电器
 */
void menu3()
{
  // 显示标题
  OLED_ShowString(128 / 2 - 9 * 4, 0, "Mula Lamp");

  // 处理无线数据
  strtok((char *)loraBuffer, ",");
  uint8_t cache = atol(strtok(NULL, ","));

  // 根据无线数据控制LED1 and LED2 与 继电器
  GpioWrite(&Switch, cache);
  GpioWrite(&Led1, !cache);
  GpioWrite(&Led2, !cache);

  // 显示对应状态
  if (cache)
    OLED_ShowString(0, 2, "Lamp:Open");
  else
    OLED_ShowString(0, 2, "Lamp:Close");
}

/**
 * @brief 发送无线数据
 * 与主控端同步显示无线通讯频率
 */
void menu4()
{
  OLED_ShowString(128 / 2 - 9 * 4, 0, "View Freq");
  OLED_ShowString(0, 2, "Freq:433000000");
}