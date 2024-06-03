/******************************************************************************
*
*
*
*
*
*
*
******************************************************************************/

#ifndef _USART1_BOARD_H_
#define _USART1_BOARD_H_

/*===========================================================================*/
#ifdef __cplusplus  /* C++支持 */
	extern "C"{
#endif
/*===========================================================================*/

/******************************************************************************
                               外部函数头文件                        
******************************************************************************/

#include "board.h" 

/*
#define USART_FLAG_CTS                       ((uint16_t)0x0200)
#define USART_FLAG_LBD                       ((uint16_t)0x0100)
#define USART_FLAG_TXE                       ((uint16_t)0x0080)
#define USART_FLAG_TC                        ((uint16_t)0x0040)
#define USART_FLAG_RXNE                      ((uint16_t)0x0020)
#define USART_FLAG_IDLE                      ((uint16_t)0x0010)
#define USART_FLAG_ORE                       ((uint16_t)0x0008)
#define USART_FLAG_NE                        ((uint16_t)0x0004)
#define USART_FLAG_FE                        ((uint16_t)0x0002)
#define USART_FLAG_PE                        ((uint16_t)0x0001)
*/

/******************************************************************************
                               定义相关应用项
******************************************************************************/

#define USART1_REC_LEN					255	//定义最大接收字节数
#define USART1_RX_TIMEOUT				3//6	//定义接收超时时间,单位：ms，接收64个字节需用时64*10*(1/115200)=5.55556ms，这里故取值6ms

/******************************************************************************
                              外部调用功能函数
******************************************************************************/

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];	//接收缓冲，最大USART1_REC_LEN个字节，末字节为换行符 
extern uint16_t USART1_RX_COUNT;					//usart1接收到数据个数
extern bool F_USART1_RX_FINISH;//接收状态标记,1:usart1接收完成，0：usart1接收未完成
extern bool F_USART1_RX_RECEIVING;//串口接收正在进行中标记,1:进行中，0：暂停或结束
extern uint8_t USART1_RX_TIMEOUT_COUNT;

/******************************************************************************
*函数名称：void USART1_Init(uint32_t bound)
*功能：初始化串口1
*输入：uint32_t bound，波特率
*输出：无
*返回：无
******************************************************************************/
void USART1_Init(uint32_t bound);

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
FlagStatus USART1_GetFlagStatus(uint16_t USART_FLAG);

/******************************************************************************
*函数名称：void USART1_SendData(uint16_t Data)
*功能：串口1发送一个字节数据
*输入：uint16_t Data，待发送的数据
*输出：无
*返回：无
******************************************************************************/
void USART1_SendData(uint16_t Data);

/******************************************************************************
*函数名称：uint16_t USART1_ReceiveData(void)
*功能：串口1接收一个字节数据
*输入：无
*输出：无
*返回：返回接收到的串口数据
******************************************************************************/
uint16_t USART1_ReceiveData(void);

/******************************************************************************
*函数名称：void USART1_SendStr(uint8_t *Data, uint8_t length)
*功能：串口1发送length个字节
*输入：uint8_t *Data-待发送的数据， uint8_t length-待发送的数据长度
*输出：无
*返回：无
******************************************************************************/
void USART1_SendStr(uint8_t *Data, uint16_t length);

/******************************************************************************
*函数名称：void USART1_ReceiveClr(void)
*功能：串口1相关寄存器和标志位清空
*输入：无
*输出：无
*返回：无
******************************************************************************/
void USART1_ReceiveClr(void);



/**********************************************************************************************
*函数：uint16_t USART1_ReadRxBuffer( uint8_t *payload)
*功能：读取USART1接收缓存信息，包括读取接收到的数据长度和缓存数据。
*输入：无
*输出：uint8_t *payload, 指针，指向要用于存放接收缓存的地址
*返回：返回接收到的数据长度
*特殊说明：每调用USART1_ReadRxBuffer()函数，USART1_RX_COUNT会被置0，F_USART1_RX_FINISH赋值为false
**********************************************************************************************/
uint16_t USART1_ReadRxBuffer( uint8_t *payload);


/******************************************************************************
*函数名称：void USART1_Process(void)
*功能：串口2进程处理
*输入：无
*输出：无
*返回：无
******************************************************************************/
void USART1_Process(void);

bool Get_F_USART1_RX_FINISH(void);

/*===========================================================================*/
#ifdef __cplusplus  /* C++支持 */
	}
#endif
/*===========================================================================*/

#endif  /* _USART2_BOARD_H_ */





