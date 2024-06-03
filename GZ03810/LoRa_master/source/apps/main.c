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

uint32_t menuID = 0;   // 用户菜单ID
uint32_t keyID = 2;    // 选择菜单ID
uint8_t LEDStatus = 0; // 模式三LED与继电器状态

void menu1();    // 模式1
void menu2();    // 模式2
void menu3();    // 模式3
void menu4();    // 模式4
void sendMode(); // 发送模式

/**
 * @brief 单片机初始化
 *
 */
void Init()
{
  BoardInitMcu();    // 初始化MCU
  BoardInitPeriph(); // 初始化片上外设
  keys_init();       // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  NS_RadioInit(433000000, 10, 1000, 1000); // 初始化无线通信

  ADCS_Init();      // ADC初始化    (光照必须)
  hal_temHumInit(); // 初始化温湿度 (温湿度必须)

  OLED_Init();       // 初始化OLED  (OLED必须)
  OLED_Display_On(); // 打开OLED  (OLED必须)
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init(); // 初始化
  while (1)
  {
    switch (menuID) // 模式选择
    {
    case 0:
      menu1(); // 模式1
      break;
    case 1:
      menu2(); // 模式2
      break;
    case 2:
      menu3(); // 模式3
      break;
    case 3:
      menu4(); // 模式4
      break;
    }
    DelayMs(100); // 系统延时
  }
}

/**
 * @brief 模式1
 * 显示模式菜单
 * 提供模式选择
 */
void menu1()
{
  // OLED显示
  OLED_ShowString(128 / 2 - 4 * 4, 0, "Menu");
  OLED_ShowString(0, 2, "1.Auto Lamp ");
  OLED_ShowString(0, 4, "2.Mula Lamp ");
  OLED_ShowString(0, 6, "3.View Freq ");

  OLED_ShowChar(12 * 8, keyID, '<'); // 显示光标

  // 按键操作光标
  if (isKey2Pressed()) // 光标上移
  {
    OLED_ShowChar(12 * 8, keyID, ' ');
    keyID -= 2;
    if (keyID <= 0)
    {
      keyID = 6;
    }
    resetKey2();
  }
  if (isKey3Pressed()) // 光标下移
  {
    OLED_ShowChar(12 * 8, keyID, ' ');
    keyID += 2;
    if (keyID > 6)
    {
      keyID = 2;
    }
    resetKey3();
  }
  if (isKey4Pressed()) // 模式选择
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
    OLED_Clear(); // 清屏
    resetKey4();
  }
  sendMode(); // 发送模式报文
}

/**
 * @brief 模式2
 * 显示实时温湿度及光照
 * 光照低于阈值点亮LED1 and LED2
 */
void menu2()
{
  // 定义数据变量
  float light, temp, hum;
  uint8_t tempData[5], humData[5], lightData[6];

  // 传感器数值
  AdcScanChannel();                             // 转换ADC双信道寄存器数据为电压值
  light = ((5.0 / 2.0) * AdcReadCh0()) * 100.0; // 通过公式获得光照
  call_sht11(&temp, &hum);                      // 获取温湿度数据

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

  // OLED显示
  OLED_ShowString(128 / 2 - 4 * 9, 0, "Auto Lamp");
  OLED_ShowString(0, 2, "Light:");
  OLED_ShowString(0, 4, "Temp:");
  OLED_ShowString(0, 6, "Humi:");

  OLED_ShowString(6 * 8, 2, lightData);
  OLED_ShowString(5 * 8, 4, tempData);
  OLED_ShowString(5 * 8, 6, humData);

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

  // 退出
  if (isKey4Pressed())
  {
    menuID = 0;
    OLED_Clear();
    resetKey4();
  }

  // 发送无线数据
  uint8_t loraBuffer[20];
  sprintf((char *)loraBuffer, "%d,%.2f,%.2f,%.2f", menuID, light, temp, hum);
  SX1276Send(loraBuffer, 20);
}

/**
 * @brief 模式3
 * 无线点对点控制节点端继电器
 */
void menu3()
{

  // OLED显示
  OLED_ShowString(128 / 2 - 9 * 4, 0, "Mula Lamp");
  if (LEDStatus)
  {
    OLED_ShowString(0, 2, "Lamp:Open");
  }
  else
  {
    OLED_ShowString(0, 2, "Lamp:Close");
  }

  // 开启LED与节点端继电器
  if (isKey2Pressed())
  {
    GpioWrite(&Led1, 0);
    GpioWrite(&Led2, 0);
    LEDStatus = 1;
    resetKey2();
  }

  // 关闭LED与节点端继电器
  if (isKey3Pressed())
  {
    GpioWrite(&Led1, 1);
    GpioWrite(&Led2, 1);
    LEDStatus = 0;
    resetKey3();
  }

  // 退出
  if (isKey4Pressed())
  {
    menuID = 0;
    OLED_Clear();
    resetKey4();
  }

  // 发送无线数据
  uint8_t loraBuffer[20];
  sprintf((char *)loraBuffer, "%d,%d", menuID, LEDStatus);
  SX1276Send(loraBuffer, 20);
}

/**
 * @brief 模式4
 * 显示无线通信频率
 */
void menu4()
{
  // OLED显示
  OLED_ShowString(128 / 2 - 9 * 4, 0, "View Freq");
  OLED_ShowString(0, 2, "Freq:433000000");
  OLED_ShowCHinese(0, 4, 52);

  // 退出
  if (isKey4Pressed())
  {
    menuID = 0;
    OLED_Clear();
    resetKey4();
  }

  // 发送无线数据
  sendMode();
}

/**
 * @brief 发送无线数据
 * 向节点端发送模式数据
 */
void sendMode()
{
  uint8_t loraBuffer[20];
  sprintf((char *)loraBuffer, "%d", menuID);
  SX1276Send(loraBuffer, 20);
}
