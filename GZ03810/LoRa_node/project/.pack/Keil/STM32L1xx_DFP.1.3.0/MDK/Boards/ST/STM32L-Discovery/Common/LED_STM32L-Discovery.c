/*-----------------------------------------------------------------------------
 * Name:    LED_STM32L-Discovery.c
 * Purpose: LED interface for STM32L-Discovery evaluation board
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2020 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

/*! \page stm32l_disco_bsi_led LED BSI Setup

The BSI LED requires:
  - Setup of pins used for LED

Pinout tab
----------
  1. Click on <b>PB7</b> pin and select <b>GPIO_Output</b> function
  2. Click on <b>PB6</b> pin and select <b>GPIO_Output</b> function
*/

#include "Board_LED.h"                  // ::Board Support:LED

#include "stm32l1xx.h"                  // Device header
#include "stm32l1xx_hal.h"              // Device HAL
#if defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
#include "MX_Device.h"
#endif

typedef struct {
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState active_state;
  uint8_t       reserved;
} PIN_CONFIG_t;

/* ------------------------------- Configuration ---------------------------- */
#define LED_NUM                         2U

#if defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
  #ifndef HAL_GPIO_MODULE_ENABLED
    #error "::Device:STM32Cube Framework (API): STM32CubeMX: Enable GPIO!"
  #endif

  #if (!defined(MX_PB7_Pin)) || (!defined(MX_PB6_Pin))
    #error "::Device:STM32Cube Framework (API): STM32CubeMX: Set pins PB7 and PB6 to GPIO_Output function!"
  #endif

  static const PIN_CONFIG_t PIN_CONFIG[LED_NUM] = {
      { MX_PB7_GPIOx, MX_PB7_GPIO_Pin, GPIO_PIN_SET, 0U }
    , { MX_PB6_GPIOx, MX_PB6_GPIO_Pin, GPIO_PIN_SET, 0U }
  };
#else
  static const PIN_CONFIG_t PIN_CONFIG[LED_NUM] = {
      { GPIOB,        GPIO_PIN_7,      GPIO_PIN_SET, 0U }
    , { GPIOB,        GPIO_PIN_6,      GPIO_PIN_SET, 0U }
  };
#endif
/* -------------------------------------------------------------------------- */

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize I/O interface for LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {
#if !defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
  GPIO_InitTypeDef  gpioinitstruct;

  __HAL_RCC_GPIOC_CLK_ENABLE();

  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

  gpioinitstruct.Pin    = PIN_CONFIG[0].pin;
  HAL_GPIO_Init(PIN_CONFIG[0].port, &gpioinitstruct);

  gpioinitstruct.Pin    = PIN_CONFIG[1].pin;
  HAL_GPIO_Init(PIN_CONFIG[1].port, &gpioinitstruct);
#else
  // All initialization code is generated by CubeMX
#endif

  return 0;
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize I/O interface for LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Uninitialize (void) {

#if !defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
  HAL_GPIO_DeInit(PIN_CONFIG[0].port, PIN_CONFIG[0].pin);

  HAL_GPIO_DeInit(PIN_CONFIG[1].port, PIN_CONFIG[1].pin);
#else
  // All de-initialization code is generated by CubeMX
#endif

  return 0;
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_On (uint32_t num) {

  if (num >= LED_NUM) {
    return -1;
  }

  if (PIN_CONFIG[num].active_state == GPIO_PIN_RESET) {
    HAL_GPIO_WritePin(PIN_CONFIG[num].port, PIN_CONFIG[num].pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(PIN_CONFIG[num].port, PIN_CONFIG[num].pin, GPIO_PIN_SET);
  }

  return 0;
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Off (uint32_t num) {

  if (num >= LED_NUM) {
    return -1;
  }

  if (PIN_CONFIG[num].active_state == GPIO_PIN_RESET) {
    HAL_GPIO_WritePin(PIN_CONFIG[num].port, PIN_CONFIG[num].pin, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(PIN_CONFIG[num].port, PIN_CONFIG[num].pin, GPIO_PIN_RESET);
  }

  return 0;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut(uint32_t value) {
  uint8_t i;

  for (i = 0U; i < LED_NUM; i++) {
    if (value & (1UL << i)) {
      LED_On(i);
    } else {
      LED_Off(i);
    }
  }

  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount (void) {
  return LED_NUM;
}
