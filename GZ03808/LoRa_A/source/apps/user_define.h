#ifndef _USER_DEFINE_H_
#define _USER_DEFINE_H_

#include <stdbool.h>

//OLED显示屏相关
#define USE_OLED_RST 				false 	//没有使用到OLED显示屏的RST复位脚

//LED灯相关
#define USE_LED3_TO_LED4 			false

//LORA模块相关
#define USE_SX1276_RESET 			false 	//没有使用该引脚
#define USE_SX1276_DIO2_TO_DIO5 	false 	//没有使用SX_1276的DIO2到DIO5的引脚
#define	USE_SX1276_RXTX 			false 	//没有使用该引脚
#define USE_RADIO_ANT_SWITCH 		false

//LORA模块SX1276/1278寄存器配置相关

//电池相关
#define USE_BATTERY 				false

//调试仿真相关
#define ENGINEER_DEBUG 				false	//调试信息开关，true时会打印printf到串口
#define USE_PIN_NC 					false
#define NEWLAND_ACCELERATE_DEBUG	true	//加速发送时间
#define NEWLAND_USE_RX_TX_RF_SET	true	//无线收发设置使用新大陆定义
#define NEWLAND_EXHIBITION			true	//展示收发传输

//常数
#define NEWLAND_LORAWAN_NETWORK_ID          (uint32_t)(0x004E4C45) //"NLE"的ASCII码值，OTAA入网时使用
#define NEWLAND_LORAWAN_SKEY 				{ 'N', 'e', 'w', 'l', 'a', 'n', 'd', ' ', 'E', 'D', 'U', '2', '0', '1', '8', '2' }//会话密钥，可以用于区分每个不同网络
#if (NEWLAND_USE_RX_TX_RF_SET != false)
#define NEWLAND_RECEIVE_DELAY1 (uint16_t)(3000)
#ifdef TYPE_CLASS_C
#define NEWLAND_RECEIVE_DELAY2 (uint16_t)(0)
#else
#define NEWLAND_RECEIVE_DELAY2 (uint16_t)(3000+NEWLAND_RECEIVE_DELAY1)
#endif /* TYPE_CLASS_C */
#endif /* (NEWLAND_USE_RX_TX_RF_SET != false) */

#endif //_USER_DEFINE_H_
