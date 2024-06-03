/**********************************************************************************************************
*
*
*
*
**********************************************************************************************************/
#ifndef _NS_RADIO_H_
#define _NS_RADIO_H_

#include "board.h"
#include "radio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             3         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define TX_TIMEOUT_VALUE                            (1000)	//码元速率906bps，113.25byte/s，128字节传输需要1S左右
#define RX_TIMEOUT_VALUE                            (1000)	//码元速率906bps，113.25byte/s，128字节传输需要1S左右
#define BUFFER_SIZE                                 255			// Define the payload size here

/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

/**********************************************************************************************
*函数：void NS_RadioEventsInit( void )
*功能：对射频模块事件回调函数进行初始化
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void NS_RadioEventsInit( void );

/**********************************************************************************************
*函数：void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout )
*功能：对射频模块发送、接收参数进行初始化
*输入：
*			freq载波频率，建议433MHz附近频率最佳
*			power发射功率，最大20
*			txTimeout发送超时时间
*输出：无
*特殊说明：无
**********************************************************************************************/
void NS_RadioSetTxRxConfig( uint32_t freq, int8_t power, uint32_t txTimeout );

/**********************************************************************************************
*函数：void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout, uint32_t rxTimeout)
*功能：对射频模块发送、接收参数进行初始化
*输入：
*			freq载波频率，建议433MHz附近频率最佳
*			power发射功率，最大20
*			txTimeout发送超时时间
*			uint32_t rxTimeout 接收超时时间
*输出：无
*特殊说明：无
**********************************************************************************************/
void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout, uint32_t rxTimeout);

/**********************************************************************************************
*函数：uint16_t ReadRadioRxBuffer( uint8_t *payload)
*功能：读取射频接收缓存信息，包括读取接收到的数据长度和缓存数据。
*输入：无
*输出：uint8_t *payload, 指针，指向要用于存放接收缓存的地址
*返回：返回接收到的数据长度
*特殊说明：每调用一次ReadRadioRxBuffer( )函数，LoRaBufferSize会被置0.
**********************************************************************************************/
uint16_t ReadRadioRxBuffer( uint8_t *payload);

/**********************************************************************************************
*函数：void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr )
*功能：读取射频信号强度和信噪比
*输入：无
*输出：int16_t *rssi,信号强度 int8_t *snr信噪比
*返回：无
*特殊说明：无
**********************************************************************************************/
void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr );

#ifdef __cplusplus
}
#endif

#endif /*_NS_RADIO_H_*/
