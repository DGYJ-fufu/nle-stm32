/**********************************************************************************************************
*使用说明：
*TIM2和TIM3接口函数，用户要使用TIM2或3的定时器中断时，先调用Tim2McuInit()或Tim3McuInit(),函数的输入参数就是中
*断周期，单位是MS，取值1~1023MS，用户的中断应用程序请添加到本文件末尾的HAL_TIM_PeriodElapsedCallback()的函数空
*白处。定时器运行后，每逢中断周期时间到便运行中断内的应用程序，并且按中断周期重复。
*
*
**********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "tim-board.h"

/** @addtogroup STM32L1xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* variables -----------------------------------------------------------------*/

/* TIM handle declaration */
TIM_HandleTypeDef    Tim2Handle;
TIM_HandleTypeDef    Tim3Handle;
TIM_HandleTypeDef    Tim4Handle;


TimerCallback timer2Callback = NULL;
TimerCallback timer3Callback = NULL;
TimerCallback timer4Callback = NULL;


TimerHandleCallback timerBaseMspInitCallback=NULL;
TimerHandleCallback timerPeriodElapsedCallback=NULL;

/* Private functions ---------------------------------------------------------*/

/**********************************************************************************************
*函数：void Tim2McuInit(uint16_t PeriodValueMs)
*功能：TIM2定时器初始化，设置中断周期PeriodValueMs毫秒
*输入：uint16_t PeriodValueMs中断周期，单位:ms，取值1~1023
*输出：无
特殊说明：定时器是16位定时器，最大定时周期是1024ms.原理：64000分频，定时1ms，周期为64-1，所以定时
*					16位最大值为65535，最大周期65536=64*X，求解X=1024
**********************************************************************************************/
void Tim2McuInit(uint16_t PeriodValueMs)
{
    uint32_t uwPrescalerValue = 0;
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM2 input clock (TIM2CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM2CLK = PCLK1
        PCLK1 = HCLK
        => TIM2CLK = HCLK = SystemCoreClock
      To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
      Prescaler = (TIM2CLK / TIM2 counter clock) - 1
      Prescaler = (SystemCoreClock /64 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIM2 counter clock equal to 64KHz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 64000) - 1;

    /* Set TIM2 instance */
    Tim2Handle.Instance = TIM2;

    /* Initialize TIM2 peripheral as follows:
         + Period = 1000 - 1
         + Prescaler = (SystemCoreClock/64000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    Tim2Handle.Init.Period            = PeriodValueMs*64 - 1;//周期PeriodValueMs毫秒
    Tim2Handle.Init.Prescaler         = uwPrescalerValue;
    Tim2Handle.Init.ClockDivision     = 0;
    Tim2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&Tim2Handle) != HAL_OK)
    {
        /* Initialization Error */
        printf("定时器2初始化失败！\r\n");
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&Tim2Handle) != HAL_OK)
    {
        /* Starting Error */
        printf("定时器2启动中断失败！\r\n");
    }
}

/**********************************************************************************************
*函数：void Tim3McuInit(uint16_t PeriodValueMs)
*功能：TIM3定时器初始化，设置中断周期PeriodValueMs毫秒
*输入：uint16_t PeriodValueMs中断周期，单位:0.25ms，取值1~1023
*输出：无
*特殊说明：定时器是16位定时器，最大定时周期是1024ms.原理：16000分频，定时0.25ms，周期为64-1，所以定时
*					16位最大值为65535，最大周期65536=64*X，求解X=1024
**********************************************************************************************/
void Tim3McuInit(uint16_t PeriodValueMs)
{
    uint32_t uwPrescalerValue = 0;
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM3CLK = PCLK1
        PCLK1 = HCLK
        => TIM3CLK = HCLK = SystemCoreClock
      To get TIM3 counter clock at 64 KHz, the Prescaler is computed as following:
      Prescaler = (TIM3CLK / TIM3 counter clock) - 1
      Prescaler = (SystemCoreClock /64 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIM3 counter clock equal to 64000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 64000) - 1;

    /* Set TIM3 instance */
    Tim3Handle.Instance = TIM3;

    /* Initialize TIM3 peripheral as follows:
         + Period = 1000 - 1
         + Prescaler = (SystemCoreClock/64000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    //Tim3Handle.Init.Period            = (PeriodValueMs*64) - 1;//周期PeriodValueMs毫秒
    Tim3Handle.Init.Period            = (PeriodValueMs*16) - 1;//周期PeriodValueMs毫秒
    Tim3Handle.Init.Prescaler         = uwPrescalerValue;
    Tim3Handle.Init.ClockDivision     = 0;
    Tim3Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&Tim3Handle) != HAL_OK)
    {
        /* Initialization Error */
        printf("定时器3初始化失败！\r\n");
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&Tim3Handle) != HAL_OK)
    {
        /* Starting Error */
        printf("定时器3启动中断失败！\r\n");
    }
}



/**********************************************************************************************
*函数：void Tim4McuInit(uint16_t PeriodValueMs)
*功能：TIM4定时器初始化，设置中断周期PeriodValueMs毫秒
*输入：uint16_t PeriodValueMs中断周期，单位:0.25ms，取值1~1023
*输出：无
*特殊说明：定时器是16位定时器，最大定时周期是1024ms.原理：16000分频，定时0.25ms，周期为64-1，所以定时
*					16位最大值为65535，最大周期65536=64*X，求解X=1024
**********************************************************************************************/
void Tim4McuInit(uint16_t PeriodValueMs)
{
    uint32_t uwPrescalerValue = 0;
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
      In this example TIM4 input clock (TIM4CLK)  is set to APB1 clock (PCLK1),
      since APB1 prescaler is equal to 1.
        TIM3CLK = PCLK1
        PCLK1 = HCLK
        => TIM3CLK = HCLK = SystemCoreClock
      To get TIM4 counter clock at 64 KHz, the Prescaler is computed as following:
      Prescaler = (TIM4CLK / TIM3 counter clock) - 1
      Prescaler = (SystemCoreClock /64 KHz) - 1

      Note:
       SystemCoreClock variable holds HCLK frequency and is defined in system_stm32l1xx.c file.
       Each time the core clock (HCLK) changes, user had to update SystemCoreClock
       variable value. Otherwise, any configuration based on this variable will be incorrect.
       This variable is updated in three ways:
        1) by calling CMSIS function SystemCoreClockUpdate()
        2) by calling HAL API function HAL_RCC_GetSysClockFreq()
        3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
    ----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIM3 counter clock equal to 64000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 64000) - 1;

    /* Set TIM3 instance */
    Tim4Handle.Instance = TIM4;

    /* Initialize TIM4 peripheral as follows:
         + Period = 1000 - 1
         + Prescaler = (SystemCoreClock/64000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    //Tim4Handle.Init.Period            = (PeriodValueMs*64) - 1;//周期PeriodValueMs毫秒
    Tim4Handle.Init.Period            = (PeriodValueMs*16) - 1;//周期PeriodValueMs毫秒
    Tim4Handle.Init.Prescaler         = uwPrescalerValue;
    Tim4Handle.Init.ClockDivision     = 0;
    Tim4Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&Tim4Handle) != HAL_OK)
    {
        /* Initialization Error */
        printf("定时器4初始化失败！\r\n");
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&Tim4Handle) != HAL_OK)
    {
        /* Starting Error */
        printf("定时器4启动中断失败！\r\n");
    }
}




/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* TIMx Peripheral clock enable */
        TIM2_CLK_ENABLE();

        /*##-2- Configure the NVIC for TIMx ########################################*/
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if(htim->Instance == TIM3)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* TIMx Peripheral clock enable */
        TIM3_CLK_ENABLE();

        /*##-2- Configure the NVIC for TIMx ########################################*/
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM3_IRQn, 3, 1);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
		else if(htim->Instance == TIM4)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* TIMx Peripheral clock enable */
        TIM4_CLK_ENABLE();

        /*##-2- Configure the NVIC for TIMx ########################################*/
        /* Set the TIMx priority */
        HAL_NVIC_SetPriority(TIM4_IRQn, 3, 1);

        /* Enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
    else
    {
        if(timerBaseMspInitCallback!=NULL)
				{
					 timerBaseMspInitCallback(htim);
        }
    }
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim2Handle);
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim3Handle);
}



/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&Tim4Handle);
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        /*TIM2用户中断应用程序请添加在下方空白处*/
        //========================》TIM2空白处开始
				//Time2Handler();
				if (timer2Callback != NULL) {
					timer2Callback();
				}
        //《========================TIM2空白处结束
    }
    else if(htim->Instance == TIM3)
    {
        /*TIM3用户中断应用程序请添加在下方*/
        //========================》TIM3空白处开始
				//Time3Handler();
				if (timer3Callback != NULL) {
					timer3Callback();
				}
        //《========================TIM3空白处结束
    }
		else if(htim->Instance == TIM4)
    {
        /*TIM3用户中断应用程序请添加在下方*/
        //========================》TIM3空白处开始
				//Time3Handler();
				if (timer4Callback != NULL) {
					timer4Callback();
				}
        //《========================TIM3空白处结束
    }
    else
    {
        if(timerPeriodElapsedCallback!=NULL)
				{
					 timerPeriodElapsedCallback(htim);
        }
    }
}


void setTimer2Callback(TimerCallback timerCallback){
	timer2Callback = timerCallback;
}

void setTimer3Callback(TimerCallback timerCallback){
	timer3Callback = timerCallback;
}

void setTimer4Callback(TimerCallback timerCallback){
	timer4Callback = timerCallback;
}

void setTimerBaseMspInitCallback(TimerHandleCallback callback){
  timerBaseMspInitCallback=callback;
}

void setTimerPeriodElapsedCallback(TimerHandleCallback callback){
   timerPeriodElapsedCallback=callback;
}

/********************************END OF FILE********************************/
