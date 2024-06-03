/* Includes ------------------------------------------------------------------*/
#include "uart_receiver.h"
#include "usart1-board.h"

void uartReceiver() {
    if(F_USART1_RX_RECEIVING)//正在接收串口数据
    {
        USART1_RX_TIMEOUT_COUNT++;//串口超时计数
        if(USART1_RX_TIMEOUT_COUNT>USART1_RX_TIMEOUT)//数据接收间隔超过3ms
        {   //串口接收完成或结束
            F_USART1_RX_RECEIVING=false;
            USART1_RX_TIMEOUT_COUNT=0;
            F_USART1_RX_FINISH=true;
        }
    }
}
