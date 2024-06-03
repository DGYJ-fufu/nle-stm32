/* Includes ------------------------------------------------------------------*/

#ifndef __LED_LIGHT_H
#define __LED_LIGHT_H

#include "stm32l1xx_hal.h"
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

void setBreathLedArg(uint8_t levelCnt, uint8_t levelUpDevide);
void pwmLevelUp(void);
void pwm(void);
void switchLed1Breath(void);
void switchLed2Breath(void);
void breathLed(void);
void startLedBreath(void);
void stopLedBreath(void);

void blinkPerHelfSecond(void);
void startLedBlink(void);
void stopLedBlink(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_LIGHT_H */


