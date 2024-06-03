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
  keys_init(); // 按键初始化
  setTimer2Callback(Time2Handler);
  Tim2McuInit(1); // 定时器初始化，设置定时中断1ms中断一次

  ADCS_Init();      // 初始化ADC
  hal_temHumInit(); // 初始化温湿度

  OLED_Init();       //  初始化OLED
  OLED_Display_On(); // 开启OLED显示

  USART1_Init(115200); // 初始化串口
}

/**
 * Main application entry point.
 */
int main(void)
{
  Init();
  while (1)
  {
    // 显示固定内容
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

    // 获取传感器数据
    float light, temp, hum;
    AdcScanChannel(); // ADC转换
    light = pow(10, ((1.78 - log10(33 / AdcReadCh0() - 10)) / 0.6));
    call_sht11(&temp, &hum);

    // 显示传感器书
    OLED_ShowNum(16 * 2 + 8, 2, temp, 2, 16);
    OLED_ShowNum(16 * 2 + 8, 4, hum, 2, 16);
    OLED_ShowNum(16 * 2 + 8, 6, light, 4, 16);

    // 显示单位
    OLED_ShowCHinese(16 * 4, 2, 62);
    OLED_ShowChar(16 * 4, 4, '%');
    OLED_ShowString(16 * 5, 6, "lux");

    // 根据光照值开关LED2
    if (light < 100)
      GpioWrite(&Led2, 0);
    else
      GpioWrite(&Led2, 1);

    // 串口发送传感器数据
    uint8_t data[50];
    memset1(data, '\0', 50);
    // uint8_t data[50] = "temperature:27|humidity:36|light:1210";    // 测试信息
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
        // cache + i * 2 为指针地址偏移,因为转成hex后需要两个位置的空间,所以需要通过偏移来预留空间
        // 当循环开始i=0,那么指针位置就会为0,也就是为最开始的位置
        // 当i=1时,此时指针位置就为2,即序列的第三的位置
        sprintf(cache + i * 2, "%02X", data[i]);
      }
      USART1_SendStr(cache, 100);
    }

    DelayMs(500);
  }
}
