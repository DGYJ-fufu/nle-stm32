/* Includes ------------------------------------------------------------------*/
#include "NS_Radio.h"
#include <string.h>
#include "board.h"
#include "radio.h"

static uint16_t LoRaBufferSize = 0;
static uint8_t LoRaBuffer[BUFFER_SIZE];
static int8_t RssiValue = 0;
static int8_t SnrValue = 0;

static RadioEvents_t RadioEvents;

/**********************************************************************************************
*函数：void NS_RadioEventsInit( void )
*功能：对射频模块事件回调函数进行初始化
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void NS_RadioEventsInit( void )
{
    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;

    Radio.Init( &RadioEvents );
}


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
void NS_RadioSetTxRxConfig( uint32_t freq, int8_t power, uint32_t txTimeout )
{
    Radio.SetChannel( freq );
    Radio.SetTxConfig( MODEM_LORA, power, 0, LORA_BANDWIDTH,
                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                       true, 0, 0, LORA_IQ_INVERSION_ON, txTimeout);
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                       0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
}

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
void NS_RadioInit( uint32_t freq, int8_t power, uint32_t txTimeout, uint32_t rxTimeout)
{
	 NS_RadioEventsInit();//无线射频模块注册事件回调函数
     NS_RadioSetTxRxConfig( freq, power, txTimeout );
	 Radio.Rx( rxTimeout );//设置LoRa模块为接收模式，同时设置超时时间，必许步骤
}

/**********************************************************************************************
*函数：uint16_t ReadRadioRxBuffer( uint8_t *payload)
*功能：读取射频接收缓存信息，包括读取接收到的数据长度和缓存数据。
*输入：无
*输出：uint8_t *payload, 指针，指向要用于存放接收缓存的地址
*返回：返回接收到的数据长度
*特殊说明：每调用一次ReadRadioRxBuffer( )函数，LoRaBufferSize会被置0.
**********************************************************************************************/
uint16_t ReadRadioRxBuffer( uint8_t *payload)
{
	  uint16_t LengthTemp;
	  LengthTemp = LoRaBufferSize;
	  if(LengthTemp <= 0)
	  {
        return 0;
	  }
	  LoRaBufferSize = 0;
    //payload = LoRaBuffer;
	  memcpy( payload, LoRaBuffer, LengthTemp );
	  return LengthTemp;
}

/**********************************************************************************************
*函数：void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr )
*功能：读取射频信号强度和信噪比
*输入：无
*输出：int16_t *rssi,信号强度 int8_t *snr信噪比
*返回：无
*特殊说明：无
**********************************************************************************************/
void ReadRadioRssiSnr( int16_t *rssi, int8_t *snr )
{
    *rssi = RssiValue;
    *snr = SnrValue;
}

void OnTxDone( void )
{
    Radio.Sleep( );
    Radio.Rx( RX_TIMEOUT_VALUE );
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    LoRaBufferSize = size;
    memcpy( LoRaBuffer, payload, LoRaBufferSize );
    RssiValue = rssi;
    SnrValue = snr;
    Radio.Rx( RX_TIMEOUT_VALUE );
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
	Radio.Rx( RX_TIMEOUT_VALUE );
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
	Radio.Rx( RX_TIMEOUT_VALUE );
}

void OnRxError( void )
{
    Radio.Sleep( );
	Radio.Rx( RX_TIMEOUT_VALUE );
}
