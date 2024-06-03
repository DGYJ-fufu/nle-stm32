/* Includes ------------------------------------------------------------------*/
#include "led_light.h"

#define LED_ILLUMINATION_LEVEL 8
#define LED_LEVEL_TIMER				LED_ILLUMINATION_LEVEL/2
#define DEVIDE_VALUE 16

uint8_t levelCount = LED_ILLUMINATION_LEVEL;
uint8_t led_levet_timer = LED_LEVEL_TIMER;
uint8_t devideCount = DEVIDE_VALUE;
uint8_t level = 1;
uint8_t timeTic = 0;
bool isUprise = true;
bool isLed1BreathEnable = false;
bool isLed2BreathEnable = false;

void setBreathLedArg(uint8_t levelCnt, uint8_t levelUpDevide){
	levelCount = levelCnt;
	led_levet_timer = levelCount/2;
	devideCount = levelUpDevide;
}

void resetLedPwm() {
    timeTic = 0;
    if (isLed1BreathEnable)
        GpioWrite( &Led1, 0 );
    if (isLed2BreathEnable)
        GpioWrite( &Led2, 0 );
}

void pwmLevelUp() {
    static unsigned long led_level_tick = 0;
    led_level_tick++;
    if (led_level_tick > led_levet_timer) {
        led_level_tick = 0;
        if (isUprise)
            level++;
        else
            level--;
        if (level > levelCount) {
            level = levelCount;
            isUprise = false;
        } else if (level == 0) {
            isUprise = true;
        }
    }
}

void pwm() {
    timeTic++;
    if (timeTic > level) {
        if (isLed1BreathEnable)
            GpioWrite( &Led1, 1 );
        if (isLed2BreathEnable)
            GpioWrite( &Led2, 1 );
    }
    if (timeTic > levelCount) {
        resetLedPwm();
    }
}

uint8_t timeDevice = 0;
void breathLed() {
    timeDevice++;
    if (timeDevice >= devideCount) {
        timeDevice = 0;
        pwmLevelUp();
    }
    pwm();
}

void switchLed1Breath() {
    isLed1BreathEnable = !isLed1BreathEnable;
}

void switchLed2Breath() {
    isLed2BreathEnable = !isLed2BreathEnable;
}

void startLedBreath() {
    isLed1BreathEnable = true;
    isLed2BreathEnable = true;
    GpioWrite( &Led1, 1 );
    GpioWrite( &Led2, 1 );
}

void stopLedBreath() {
    isLed1BreathEnable = false;
    isLed2BreathEnable = false;
    GpioWrite( &Led1, 1 );
    GpioWrite( &Led2, 1 );
}

#define HELF_SECOND 50
uint16_t helfSecondCnt = 0;
bool isLed1Lighted = true;
bool isHelfSecondLedBlinkEnable = false;
void blinkPerHelfSecond() {
    if (!isHelfSecondLedBlinkEnable)
        return;

    helfSecondCnt ++;
    if (helfSecondCnt > HELF_SECOND) {
        helfSecondCnt = 0;
        isLed1Lighted = !isLed1Lighted;
    }

    if (isLed1Lighted) {
        GpioWrite( &Led1, 0 );
        GpioWrite( &Led2, 1 );
    }
    else {
        GpioWrite( &Led1, 1 );
        GpioWrite( &Led2, 0 );
    }
}

void startLedBlink() {
    isHelfSecondLedBlinkEnable = true;
    GpioWrite( &Led1, 1 );
    GpioWrite( &Led2, 1 );
}

void stopLedBlink() {
    isHelfSecondLedBlinkEnable = false;
    GpioWrite( &Led1, 1 );
    GpioWrite( &Led2, 1 );
}
