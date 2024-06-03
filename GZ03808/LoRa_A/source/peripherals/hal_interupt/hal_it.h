/* Includes ------------------------------------------------------------------*/

#ifndef __HAL_IT_H
#define __HAL_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

void InitPA2Irq(void);
typedef void (* IrqCallback)(void);
void setIrqCallback(IrqCallback timerCallback);
	 
#ifdef __cplusplus
}
#endif

#endif /* __HAL_IT_H */


