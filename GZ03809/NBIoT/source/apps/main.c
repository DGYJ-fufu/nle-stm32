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
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  // ADCS_Init();
  hal_temHumInit(); //  初始化温湿度

  OLED_Init();       // 初始化OLED
  OLED_Display_On(); // 开启OLED

  USART1_Init(115200); // 初始化串口1
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {
    float temp, hum;         // 定义数据变量
    call_sht11(&temp, &hum); // 获取温湿度

    // 显示标题
    for (int i = 0; i < 5; i++)
    {
      OLED_ShowCHinese((128 / 2 - 16 * 2 - 8) + 16 * i, 0, 52 + i);
    }

    // 显示温度
    OLED_ShowCHinese(0, 2, 57);
    OLED_ShowCHinese(17, 2, 58);
    OLED_ShowNum(16 * 2 + 8, 2, (int)temp, 2, 16);
    OLED_ShowCHinese(16 * 3 + 8, 2, 60);

    // 显示湿度
    OLED_ShowCHinese(0, 4, 59);
    OLED_ShowCHinese(17, 4, 58);
    OLED_ShowNum(16 * 2 + 8, 4, (int)hum, 2, 16);
    OLED_ShowChar(16 * 3 + 8, 4, '%');

    uint8_t data[100]; // 串口数据缓存区

    // 根据温度开启LED2并通过串口发送数据
    if (temp < 28)
    {
      GpioWrite(&Led2, 0);
      sprintf((char *)data, "Temperature=%.6f℃, Humidity=%.6f%%\nTemperature=%d℃, Humidity=%d%%\nLED状态:开启", temp, hum, (int)temp, (int)hum);
    }
    else
    {
      GpioWrite(&Led2, 1);
      sprintf((char *)data, "Temperature=%.6f℃, Humidity=%.6f%%\nTemperature=%d℃, Humidity=%d%%\nLED状态:关闭", temp, hum, (int)temp, (int)hum);
    }
    USART1_SendStr(&data, 90); // 串口发送数据

    Delay(1); // 延时1s
  }
}
