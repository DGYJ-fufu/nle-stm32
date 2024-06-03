/*-----------------------------------------------------------------------------
 * Name:    Buttons_STM32L-Discovery.c
 * Purpose: Buttons interface for STM32L-Discovery evaluation board
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

/*! \page stm32l_disco_bsi_buttons Buttons BSI Setup

The BSI Buttons requires:
  - Setup of pins used for buttons

Pinout tab
----------
  1. Click on <b>PA0</b> pin and select <b>GPIO_Input</b> function
*/

#include "Board_Buttons.h"              // ::Board Support:Buttons

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
#define BUTTON_NUM                      1U

#if defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
  #ifndef HAL_GPIO_MODULE_ENABLED
    #error "::Device:STM32Cube Framework (API): STM32CubeMX: Enable GPIO!"
  #endif

  #ifndef MX_PA0_WKUP1_Pin
    #error "::Device:STM32Cube Framework (API): STM32CubeMX: Set pin PA0 to GPIO_Input function!"
  #endif

  static const PIN_CONFIG_t PIN_CONFIG[BUTTON_NUM] = {
      { MX_PA0_WKUP1_GPIOx, MX_PA0_WKUP1_GPIO_Pin, GPIO_PIN_RESET, 0U }
  };
#else
  static const PIN_CONFIG_t PIN_CONFIG[BUTTON_NUM] = {
      { GPIOA,              GPIO_PIN_0,            GPIO_PIN_RESET, 0U }
  };
#endif
/* -------------------------------------------------------------------------- */

/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize I/O interface for buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) {
#if !defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
  GPIO_InitTypeDef  gpioinitstruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  gpioinitstruct.Mode   = GPIO_MODE_INPUT;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;

  gpioinitstruct.Pin    = PIN_CONFIG[0].pin;
  HAL_GPIO_Init(PIN_CONFIG[0].port, &gpioinitstruct);
#else
  // All initialization code is generated by CubeMX
#endif

  return 0;
}

/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize I/O interface for buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Uninitialize (void) {

#if !defined (RTE_DEVICE_FRAMEWORK_CUBE_MX)
  HAL_GPIO_DeInit(PIN_CONFIG[0].port, PIN_CONFIG[0].pin);
#else
  // All de-initialization code is generated by CubeMX
#endif

  return 0;
}

/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) {
  uint32_t val;
  uint8_t  i;

  val = 0U;
  for (i = 0U; i < BUTTON_NUM; i++) {
    if (HAL_GPIO_ReadPin(PIN_CONFIG[i].port, PIN_CONFIG[i].pin) == PIN_CONFIG[i].active_state) {
      val |= 1UL << i;
    }
  }

  return val;
}

/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return BUTTON_NUM;
}