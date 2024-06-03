/******************************************************************************
*
*
*
*
*
*
*
******************************************************************************/

#include "usart1-board.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];	//接收缓冲,最大USART1_REC_LEN个字节
uint16_t USART1_RX_COUNT=0;	//接收到的有效字节数目，最大USART1_REC_LEN字节
bool F_USART1_RX_FINISH=false;//接收状态标记,1:usart1接收完成，0：usart1接收未完成
bool F_USART1_RX_RECEIVING=false;//串口接收正在进行中标记,1:进行中，0：暂停或结束
uint8_t USART1_RX_TIMEOUT_COUNT=0;

/*!
 * Timer to handle the state of Usart1
 */
static TimerEvent_t Usart1Timer;

/*!
 * \brief Function executed on Usart1 Receive Timeout event
 */
static void OnUsart1TimerEvent( void )
{
	//Usart1数据接收间隔超过3ms
    TimerStop( &Usart1Timer );
    //...
	if(F_USART1_RX_RECEIVING)//正在接收串口数据
	{
		F_USART1_RX_RECEIVING=false;
		F_USART1_RX_FINISH=true;
	}
}

/******************************************************************************
*函数名称：void USART1_Init(uint32_t bound)
*功能：初始化串口1
*输入：uint32_t bound，波特率
*输出：无
*返回：无
******************************************************************************/
void USART1_Init(uint32_t bound)
{
	//初始化串口1
	UartInit( &Uart1, UART_1, UART_TX, UART_RX );	
	UartConfig( &Uart1, RX_TX, bound, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );// RX_TX
	//初始化接收超时定时器事件
	TimerInit( &Usart1Timer, OnUsart1TimerEvent );
	TimerSetValue( &Usart1Timer, USART1_RX_TIMEOUT );
}

/******************************************************************************
* 函数名称：bool USART1_GetFlagStatus(uint16_t USART_FLAG)
* 功能：Checks whether the specified USART flag is set or not.
* 输入：uint16_t USART_FLAG，USART_FLAG: specifies the flag to check.
*   This parameter can be one of the following values:
*     @arg USART_FLAG_LBD:  LIN Break detection flag
*     @arg USART_FLAG_TXE:  Transmit data register empty flag
*     @arg USART_FLAG_TC:   Transmission Complete flag
*     @arg USART_FLAG_RXNE: Receive data register not empty flag
*     @arg USART_FLAG_IDLE: Idle Line detection flag
*     @arg USART_FLAG_ORE:  OverRun Error flag
*     @arg USART_FLAG_NE:   Noise Error flag
*     @arg USART_FLAG_FE:   Framing Error flag
*     @arg USART_FLAG_PE:   Parity Error flag
* 输出：无
* 返回： The new state of USART_FLAG (SET or RESET).
******************************************************************************/
FlagStatus USART1_GetFlagStatus(uint16_t USART_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  if ((USART1->SR & USART_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/******************************************************************************
*函数名称：void USART1_SendData(uint16_t Data)
*功能：串口1发送一个字节数据
*输入：uint16_t Data，待发送的数据
*输出：无
*返回：无
******************************************************************************/
void USART1_SendData(uint16_t Data)
{
  /*
  //等待发送完成
  while(USART1_GetFlagStatus(USART_FLAG_TC) != SET);
  */
  //Transmit Data
  USART1->DR = (Data & (uint16_t)0x01FF);
}

/******************************************************************************
*函数名称：uint16_t USART1_ReceiveData(void)
*功能：串口1接收一个字节数据
*输入：无
*输出：无
*返回：返回接收到的串口数据
******************************************************************************/
uint16_t USART1_ReceiveData(void)
{
  /* Receive Data */
  return (uint16_t)(USART1->DR & (uint16_t)0x01FF);
}

/******************************************************************************
*函数名称：void USART1_SendStr(uint8_t *Data, uint8_t length)
*功能：串口1发送length个字节
*输入：uint8_t *Data-待发送的数据， uint8_t length-待发送的数据长度
*输出：无
*返回：无
******************************************************************************/
void USART1_SendStr(uint8_t *Data, uint16_t length)
{
	while(length--)
	{
		//等待发送完成
		while(USART1_GetFlagStatus(USART_FLAG_TC) != SET);
		USART1_SendData((uint16_t )(*Data));
		//等待发送完成
		while(USART1_GetFlagStatus(USART_FLAG_TC) != SET);
		Data++;
	}
}

/******************************************************************************
*函数名称：void USART1_ReceiveClr(void)
*功能：串口1相关寄存器和标志位清空
*输入：无
*输出：无
*返回：无
******************************************************************************/
void USART1_ReceiveClr(void)
{
	/*for(uint8_t i=0; i<USART1_REC_LEN; i++)//接收缓冲,最大USART2_REC_LEN个字节
	{
		USART1_RX_BUF[i]=0x00;
	}*/
	USART1_RX_COUNT=0;	//接收到的有效字节数目，最大USART1_REC_LEN字节
	F_USART1_RX_FINISH=false;//接收状态标记,1:usart1接收完成，0：usart1接收未完成
	memset(USART1_RX_BUF, 0, USART1_REC_LEN);
	//F_USART1_RX_RECEIVING=false;//串口接收正在进行中标记,1:进行中，0：暂停或结束
}


/**********************************************************************************************
*函数：uint16_t USART1_ReadRxBuffer( uint8_t *payload)
*功能：读取USART1接收缓存信息，包括读取接收到的数据长度和缓存数据。
*输入：无
*输出：uint8_t *payload, 指针，指向要用于存放接收缓存的地址
*返回：返回接收到的数据长度
*特殊说明：每调用USART1_ReadRxBuffer()函数，USART1_RX_COUNT会被置0，F_USART1_RX_FINISH赋值为false，
USART1_RX_BUF缓存区会被清0。
**********************************************************************************************/
uint16_t USART1_ReadRxBuffer( uint8_t *payload)
{
    uint16_t LengthTemp;
    LengthTemp = USART1_RX_COUNT;
    if(LengthTemp <= 0)
    {
        return 0;
    }
    memcpy( payload, USART1_RX_BUF, LengthTemp );
    USART1_ReceiveClr();
    return LengthTemp;
}



/******************************************************************************
*函数名称：void USART1_Process(void)
*功能：串口2进程处理
*输入：无
*输出：无
*返回：无
******************************************************************************/
void USART1_Process(void)
{
	if(F_USART1_RX_RECEIVING != false)
	{TimerStart( &Usart1Timer );}
}

/******************************************************************************
*函数名称：void USART1_IRQHandler(void)
*功能：串口1中断服务程序
*输入：无
*输出：无
*返回：无
******************************************************************************/
void USART1_IRQHandler(void)
{
	if(USART1_GetFlagStatus((uint16_t)USART_IT_RXNE) != RESET)  //接收到数据
	{
		USART1_RX_BUF[USART1_RX_COUNT] = (uint8_t)USART1_ReceiveData();	//读取接收到的数据
		USART1_RX_COUNT++;
		USART1_RX_TIMEOUT_COUNT=0;
		F_USART1_RX_RECEIVING=true;//正在接收串口数据
		if(USART1_RX_COUNT>=USART1_REC_LEN)//缓存区溢出
		{USART1_RX_COUNT = 0x0000;}
		F_USART1_RX_RECEIVING=true;//正在接收串口数据
	}
}

bool Get_F_USART1_RX_FINISH(){
	return F_USART1_RX_FINISH;
}
