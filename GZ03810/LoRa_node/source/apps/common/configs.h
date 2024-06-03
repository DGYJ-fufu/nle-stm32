/* Includes ------------------------------------------------------------------*/

#ifndef __CONFIG_H
#define __CONFIG_H

#include "stm32l1xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#define DEFAULT_RF_FREQUENCY 434000000
	 
static enum
{
    TemperatureDev = 0,
    HumidityDev = 1,
    FlammableGasDev = 2,
    CarbonMonoxideDev = 3,
    IlluminateDev = 4,
    HumanBodyDev = 5,
    FireDev = 6,
} ESensorType;

static enum
{
    SetPage,
    LoraFreqPage,
    SensorPage,
    MainPage,
} SetObj;

static enum
{
    TransByLora = 0,
    TransByUart = 1,
} ETransDev;

void ReadFlashCfg(void);
uint8_t getSensorType(void);
void setSensorType(uint8_t sensorTypeInput);

uint8_t getCurrentPage(void);
void setCurrentPage(uint8_t currentPageInput);

uint8_t getChannelNum(void);
void setChannelNum(uint8_t channelNumInput);
void channelNumPP(void);
void channelNumSS(void);
uint32_t getRfFrequency(void);
void setRfFrequency(uint32_t rf_frequency);

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */


