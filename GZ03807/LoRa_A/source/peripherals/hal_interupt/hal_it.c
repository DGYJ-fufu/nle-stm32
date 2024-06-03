/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "hal_it.h"
#include "board.h"

Gpio_t Int;
IrqCallback irqCallback = NULL;

void SX1276OnPa2Irq( void )
{
	if (irqCallback != NULL)
		irqCallback();
}

void setIrqCallback(IrqCallback pIrqCallback){
	irqCallback = pIrqCallback;
}

void InitPA2Irq(void){
  GpioInit( &Int, INT, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
	GpioSetInterrupt(&Int, IRQ_FALLING_EDGE, IRQ_HIGH_PRIORITY, SX1276OnPa2Irq );
}
