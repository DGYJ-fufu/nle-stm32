/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2016 Semtech

Description: Board UART driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "board.h"

#include "uart-board.h"

UART_HandleTypeDef UartHandle;
UART_HandleTypeDef Uart2Handle;
UART_HandleTypeDef Uart3Handle;

uint8_t RxData = 0;
uint8_t TxData = 0;
uint8_t Rx2Data = 0;
uint8_t Tx2Data = 0;
uint8_t Rx3Data = 0;
uint8_t Tx3Data = 0;

void UartMcuInit( Uart_t *obj, uint8_t uartId, PinNames tx, PinNames rx )
{
    obj->UartId = uartId;

	if(obj->UartId == UART_1)
	{
		__HAL_RCC_USART1_FORCE_RESET( );
		__HAL_RCC_USART1_RELEASE_RESET( );
		__HAL_RCC_USART1_CLK_ENABLE( );
		
		GpioInit( &obj->Tx, tx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF7_USART1 );
		GpioInit( &obj->Rx, rx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF7_USART1 );
	}
	else if(obj->UartId == UART_2)
	{
		__HAL_RCC_USART2_FORCE_RESET( );
		__HAL_RCC_USART2_RELEASE_RESET( );
		__HAL_RCC_USART2_CLK_ENABLE( );
		
		GpioInit( &obj->Tx, tx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF7_USART2 );
		GpioInit( &obj->Rx, rx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF7_USART2 );
	}
	else if(obj->UartId == UART_3)
	{
		__HAL_RCC_USART3_FORCE_RESET( );
		__HAL_RCC_USART3_RELEASE_RESET( );
		__HAL_RCC_USART3_CLK_ENABLE( );
		
		GpioInit( &obj->Tx, tx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF7_USART3 );
		GpioInit( &obj->Rx, rx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF7_USART3 );
	}
}

void UartMcuConfig( Uart_t *obj, UartMode_t mode, uint32_t baudrate, WordLength_t wordLength, StopBits_t stopBits, Parity_t parity, FlowCtrl_t flowCtrl )
{
	if(obj->UartId == UART_1)
	{
		UartHandle.Instance = USART1;
		UartHandle.Init.BaudRate = baudrate;
	}
	else if(obj->UartId == UART_2)
	{
		Uart2Handle.Instance = USART2;
		Uart2Handle.Init.BaudRate = baudrate;
	}
	else if(obj->UartId == UART_3)
	{
		Uart3Handle.Instance = USART3;
		Uart3Handle.Init.BaudRate = baudrate;
	}
	
	if(obj->UartId == UART_1)
	{
		if( mode == TX_ONLY )
		{
			if( obj->FifoTx.Data == NULL )
			{
				assert_param( FAIL );
			}
			UartHandle.Init.Mode = UART_MODE_TX;
		}
		else if( mode == RX_ONLY )
		{
			if( obj->FifoRx.Data == NULL )
			{
				assert_param( FAIL );
			}
			UartHandle.Init.Mode = UART_MODE_RX;
		}
		else if( mode == RX_TX )
		{
			if( ( obj->FifoTx.Data == NULL ) || ( obj->FifoRx.Data == NULL ) )
			{
				assert_param( FAIL );
			}
			UartHandle.Init.Mode = UART_MODE_TX_RX;
		}
		else
		{
		   assert_param( FAIL );
		}
	}
	else if(obj->UartId == UART_2)
	{
		if( mode == TX_ONLY )
		{
			if( obj->FifoTx.Data == NULL )
			{
				assert_param( FAIL );
			}
			Uart2Handle.Init.Mode = UART_MODE_TX;
		}
		else if( mode == RX_ONLY )
		{
			if( obj->FifoRx.Data == NULL )
			{
				assert_param( FAIL );
			}
			Uart2Handle.Init.Mode = UART_MODE_RX;
		}
		else if( mode == RX_TX )
		{
			if( ( obj->FifoTx.Data == NULL ) || ( obj->FifoRx.Data == NULL ) )
			{
				assert_param( FAIL );
			}
			Uart2Handle.Init.Mode = UART_MODE_TX_RX;
		}
		else
		{
		   assert_param( FAIL );
		}
	}
	else if(obj->UartId == UART_3)
	{
		if( mode == TX_ONLY )
		{
			if( obj->FifoTx.Data == NULL )
			{
				assert_param( FAIL );
			}
			Uart3Handle.Init.Mode = UART_MODE_TX;
		}
		else if( mode == RX_ONLY )
		{
			if( obj->FifoRx.Data == NULL )
			{
				assert_param( FAIL );
			}
			Uart3Handle.Init.Mode = UART_MODE_RX;
		}
		else if( mode == RX_TX )
		{
			if( ( obj->FifoTx.Data == NULL ) || ( obj->FifoRx.Data == NULL ) )
			{
				assert_param( FAIL );
			}
			Uart3Handle.Init.Mode = UART_MODE_TX_RX;
		}
		else
		{
		   assert_param( FAIL );
		}
	}

	if(obj->UartId == UART_1)
	{
		if( wordLength == UART_8_BIT )
		{
			UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
		}
		else if( wordLength == UART_9_BIT )
		{
			UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
		}
	}
	else if(obj->UartId == UART_2)
	{
		if( wordLength == UART_8_BIT )
		{
			Uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;
		}
		else if( wordLength == UART_9_BIT )
		{
			Uart2Handle.Init.WordLength = UART_WORDLENGTH_9B;
		}
	}
	else if(obj->UartId == UART_3)
	{
		if( wordLength == UART_8_BIT )
		{
			Uart3Handle.Init.WordLength = UART_WORDLENGTH_8B;
		}
		else if( wordLength == UART_9_BIT )
		{
			Uart3Handle.Init.WordLength = UART_WORDLENGTH_9B;
		}
	}
    
	if(obj->UartId == UART_1)
	{
		switch( stopBits )
		{
		case UART_2_STOP_BIT:
			UartHandle.Init.StopBits = UART_STOPBITS_2;
			break;
		case UART_1_STOP_BIT:
		default:
			UartHandle.Init.StopBits = UART_STOPBITS_1;
			break;
		}
	}
	else if(obj->UartId == UART_2)
	{
		switch( stopBits )
		{
		case UART_2_STOP_BIT:
			Uart2Handle.Init.StopBits = UART_STOPBITS_2;
			break;
		case UART_1_STOP_BIT:
		default:
			Uart2Handle.Init.StopBits = UART_STOPBITS_1;
			break;
		}
	}
	else if(obj->UartId == UART_3)
	{
		switch( stopBits )
		{
		case UART_2_STOP_BIT:
			Uart3Handle.Init.StopBits = UART_STOPBITS_2;
			break;
		case UART_1_STOP_BIT:
		default:
			Uart3Handle.Init.StopBits = UART_STOPBITS_1;
			break;
		}
	}
    
	if(obj->UartId == UART_1)
	{
		if( parity == NO_PARITY )
		{
			UartHandle.Init.Parity = UART_PARITY_NONE;
		}
		else if( parity == EVEN_PARITY )
		{
			UartHandle.Init.Parity = UART_PARITY_EVEN;
		}
		else
		{
			UartHandle.Init.Parity = UART_PARITY_ODD;
		}
	}
	else if(obj->UartId == UART_2)
	{
		if( parity == NO_PARITY )
		{
			Uart2Handle.Init.Parity = UART_PARITY_NONE;
		}
		else if( parity == EVEN_PARITY )
		{
			Uart2Handle.Init.Parity = UART_PARITY_EVEN;
		}
		else
		{
			Uart2Handle.Init.Parity = UART_PARITY_ODD;
		}
	}
	else if(obj->UartId == UART_3)
	{
		if( parity == NO_PARITY )
		{
			Uart3Handle.Init.Parity = UART_PARITY_NONE;
		}
		else if( parity == EVEN_PARITY )
		{
			Uart3Handle.Init.Parity = UART_PARITY_EVEN;
		}
		else
		{
			Uart3Handle.Init.Parity = UART_PARITY_ODD;
		}
	}
    
	if(obj->UartId == UART_1)
	{
		if( flowCtrl == NO_FLOW_CTRL )
		{
			UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		}
		else if( flowCtrl == RTS_FLOW_CTRL )
		{
			UartHandle.Init.HwFlowCtl = UART_HWCONTROL_RTS;
		}
		else if( flowCtrl == CTS_FLOW_CTRL )
		{
			UartHandle.Init.HwFlowCtl = UART_HWCONTROL_CTS;
		}
		else if( flowCtrl == RTS_CTS_FLOW_CTRL )
		{
			UartHandle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
		}

		UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

		if( HAL_UART_Init( &UartHandle ) != HAL_OK )
		{
			assert_param( FAIL );
		}

		HAL_NVIC_SetPriority( USART1_IRQn, 8, 0 );
		HAL_NVIC_EnableIRQ( USART1_IRQn );

		/* Enable the UART Data Register not empty Interrupt */
		HAL_UART_Receive_IT( &UartHandle, &RxData, 1 );
	}
	else if(obj->UartId == UART_2)
	{
		if( flowCtrl == NO_FLOW_CTRL )
		{
			Uart2Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		}
		else if( flowCtrl == RTS_FLOW_CTRL )
		{
			Uart2Handle.Init.HwFlowCtl = UART_HWCONTROL_RTS;
		}
		else if( flowCtrl == CTS_FLOW_CTRL )
		{
			Uart2Handle.Init.HwFlowCtl = UART_HWCONTROL_CTS;
		}
		else if( flowCtrl == RTS_CTS_FLOW_CTRL )
		{
			Uart2Handle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
		}

		Uart2Handle.Init.OverSampling = UART_OVERSAMPLING_16;

		if( HAL_UART_Init( &Uart2Handle ) != HAL_OK )
		{
			assert_param( FAIL );
		}

		HAL_NVIC_SetPriority( USART2_IRQn, 8, 0 );
		HAL_NVIC_EnableIRQ( USART2_IRQn );

		/* Enable the UART Data Register not empty Interrupt */
		HAL_UART_Receive_IT( &Uart2Handle, &Rx2Data, 1 );
	}
	else if(obj->UartId == UART_3)
	{
		if( flowCtrl == NO_FLOW_CTRL )
		{
			Uart3Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		}
		else if( flowCtrl == RTS_FLOW_CTRL )
		{
			Uart3Handle.Init.HwFlowCtl = UART_HWCONTROL_RTS;
		}
		else if( flowCtrl == CTS_FLOW_CTRL )
		{
			Uart3Handle.Init.HwFlowCtl = UART_HWCONTROL_CTS;
		}
		else if( flowCtrl == RTS_CTS_FLOW_CTRL )
		{
			Uart3Handle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
		}

		Uart3Handle.Init.OverSampling = UART_OVERSAMPLING_16;

		if( HAL_UART_Init( &Uart3Handle ) != HAL_OK )
		{
			assert_param( FAIL );
		}

		HAL_NVIC_SetPriority( USART3_IRQn, 8, 0 );
		HAL_NVIC_EnableIRQ( USART3_IRQn );

		/* Enable the UART Data Register not empty Interrupt */
		HAL_UART_Receive_IT( &Uart3Handle, &Rx3Data, 1 );
	}
}

void UartMcuDeInit( Uart_t *obj )
{
	if(obj->UartId == UART_1)
	{
		__HAL_RCC_USART1_FORCE_RESET( );
		__HAL_RCC_USART1_RELEASE_RESET( );
		__HAL_RCC_USART1_CLK_DISABLE( );
	}
	else if(obj->UartId == UART_2)
	{
		__HAL_RCC_USART2_FORCE_RESET( );
		__HAL_RCC_USART2_RELEASE_RESET( );
		__HAL_RCC_USART2_CLK_DISABLE( );
	}
	else if(obj->UartId == UART_3)
	{
		__HAL_RCC_USART3_FORCE_RESET( );
		__HAL_RCC_USART3_RELEASE_RESET( );
		__HAL_RCC_USART3_CLK_DISABLE( );
	}

    GpioInit( &obj->Tx, obj->Tx.pin, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
    GpioInit( &obj->Rx, obj->Rx.pin, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
}

//uint8_t UartMcuPutChar( Uart_t *obj, uint8_t data )
//{
//    BoardDisableIrq( );
//	if(obj->UartId == UART_1)
//	{
//		TxData = data;

//		if( IsFifoFull( &obj->FifoTx ) == false )
//		{
//			FifoPush( &obj->FifoTx, TxData );

//			// Trig UART Tx interrupt to start sending the FIFO contents.
//			__HAL_UART_ENABLE_IT( &UartHandle, UART_IT_TC );

//			BoardEnableIrq( );
//			return 0; // OK
//		}
//		BoardEnableIrq( );
//		return 1; // Busy
//	}
//	else if(obj->UartId == UART_2)
//	{
//		Tx2Data = data;

//		if( IsFifoFull( &obj->FifoTx ) == false )
//		{
//			FifoPush( &obj->FifoTx, Tx2Data );

//			// Trig UART Tx interrupt to start sending the FIFO contents.
//			__HAL_UART_ENABLE_IT( &Uart2Handle, UART_IT_TC );

//			BoardEnableIrq( );
//			return 0; // OK
//		}
//		BoardEnableIrq( );
//		return 1; // Busy
//	}
//	else //if(obj->UartId == UART_3)
//	{
//		Tx3Data = data;

//		if( IsFifoFull( &obj->FifoTx ) == false )
//		{
//			FifoPush( &obj->FifoTx, Tx3Data );

//			// Trig UART Tx interrupt to start sending the FIFO contents.
//			__HAL_UART_ENABLE_IT( &Uart3Handle, UART_IT_TC );

//			BoardEnableIrq( );
//			return 0; // OK
//		}
//		BoardEnableIrq( );
//		return 1; // Busy
//	}
//}

//uint8_t UartMcuGetChar( Uart_t *obj, uint8_t *data )
//{
//    BoardDisableIrq( );

//    if( IsFifoEmpty( &obj->FifoRx ) == false )
//    {
//        *data = FifoPop( &obj->FifoRx );
//        BoardEnableIrq( );
//        return 0;
//    }
//    BoardEnableIrq( );
//    return 1;
//}

//void HAL_UART_TxCpltCallback( UART_HandleTypeDef *handle )
//{
//    if( IsFifoEmpty( &Uart1.FifoTx ) == false )
//    {
//        TxData = FifoPop( &Uart1.FifoTx );
//        //  Write one byte to the transmit data register
//        HAL_UART_Transmit_IT( &UartHandle, &TxData, 1 );
//    }
//	else if( IsFifoEmpty( &Uart2.FifoTx ) == false )
//    {
//        Tx2Data = FifoPop( &Uart2.FifoTx );
//        //  Write one byte to the transmit data register
//        HAL_UART_Transmit_IT( &Uart2Handle, &Tx2Data, 1 );
//    }
//	else if( IsFifoEmpty( &Uart3.FifoTx ) == false )
//    {
//        Tx3Data = FifoPop( &Uart3.FifoTx );
//        //  Write one byte to the transmit data register
//        HAL_UART_Transmit_IT( &Uart3Handle, &Tx3Data, 1 );
//    }

//    if( Uart1.IrqNotify != NULL )
//    {
//        Uart1.IrqNotify( UART_NOTIFY_TX );
//    }
//	else if( Uart2.IrqNotify != NULL )
//    {
//        Uart2.IrqNotify( UART_NOTIFY_TX );
//    }
//	else if( Uart3.IrqNotify != NULL )
//    {
//        Uart3.IrqNotify( UART_NOTIFY_TX );
//    }
//}

//void HAL_UART_RxCpltCallback( UART_HandleTypeDef *handle )
//{
//    if( IsFifoFull( &Uart1.FifoRx ) == false )
//    {
//        // Read one byte from the receive data register
//        FifoPush( &Uart1.FifoRx, RxData );
//    }
//	else if( IsFifoFull( &Uart2.FifoRx ) == false )
//    {
//        // Read one byte from the receive data register
//        FifoPush( &Uart2.FifoRx, Rx2Data );
//    }
//	else if( IsFifoFull( &Uart3.FifoRx ) == false )
//    {
//        // Read one byte from the receive data register
//        FifoPush( &Uart3.FifoRx, Rx3Data );
//    }

//    if( Uart1.IrqNotify != NULL )
//    {
//        Uart1.IrqNotify( UART_NOTIFY_RX );
//    }
//	else if( Uart2.IrqNotify != NULL )
//    {
//        Uart2.IrqNotify( UART_NOTIFY_RX );
//    }
//	else if( Uart3.IrqNotify != NULL )
//    {
//        Uart3.IrqNotify( UART_NOTIFY_RX );
//    }

//	if(UartHandle.Instance == USART1)
//	{
//		HAL_UART_Receive_IT( &UartHandle, &RxData, 1 );
//	}
//	else if(Uart2Handle.Instance == USART2)
//	{
//		HAL_UART_Receive_IT( &Uart2Handle, &Rx2Data, 1 );
//	}
//	else if(Uart3Handle.Instance == USART3)
//	{
//		HAL_UART_Receive_IT( &Uart3Handle, &Rx3Data, 1 );
//	}
//}

//void HAL_UART_ErrorCallback( UART_HandleTypeDef *handle )
//{
//	if(UartHandle.Instance == USART1)
//	{
//		HAL_UART_Receive_IT( &UartHandle, &RxData, 1 );
//	}
//	else if(Uart2Handle.Instance == USART2)
//	{
//		HAL_UART_Receive_IT( &Uart2Handle, &Rx2Data, 1 );
//	}
//	else if(Uart3Handle.Instance == USART3)
//	{
//		HAL_UART_Receive_IT( &Uart3Handle, &Rx3Data, 1 );
//	}
//}

/*void USART1_IRQHandler( void )
{
    HAL_UART_IRQHandler( &UartHandle );
}

void USART2_IRQHandler( void )
{
    HAL_UART_IRQHandler( &Uart2Handle );
}

void USART3_IRQHandler( void )
{
    HAL_UART_IRQHandler( &Uart3Handle );
}
*/
