/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_timebase_tim.c
  * @brief   HAL time base based on the hardware TIM.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
<<<<<<< HEAD
TIM_HandleTypeDef        htim5;
=======
TIM_HandleTypeDef        htim11;
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
<<<<<<< HEAD
  * @brief  This function configures the TIM5 as a time base source.
=======
  * @brief  This function configures the TIM11 as a time base source.
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
  *         The time source is configured  to have 1ms time base with a dedicated
  *         Tick interrupt priority.
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  RCC_ClkInitTypeDef    clkconfig;
<<<<<<< HEAD
  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
=======
  uint32_t              uwTimclock = 0U;
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428

  uint32_t              uwPrescalerValue = 0U;
  uint32_t              pFLatency;

  HAL_StatusTypeDef     status;

<<<<<<< HEAD
  /* Enable TIM5 clock */
  __HAL_RCC_TIM5_CLK_ENABLE();
=======
  /* Enable TIM11 clock */
  __HAL_RCC_TIM11_CLK_ENABLE();
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

<<<<<<< HEAD
  /* Get APB1 prescaler */
  uwAPB1Prescaler = clkconfig.APB1CLKDivider;
  /* Compute TIM5 clock */
  if (uwAPB1Prescaler == RCC_HCLK_DIV1)
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
  }

  /* Compute the prescaler value to have TIM5 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM5 */
  htim5.Instance = TIM5;

  /* Initialize TIMx peripheral as follow:
   * Period = [(TIM5CLK/1000) - 1]. to have a (1/1000) s time base.
=======
  /* Compute TIM11 clock */
      uwTimclock = HAL_RCC_GetPCLK2Freq();

  /* Compute the prescaler value to have TIM11 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM11 */
  htim11.Instance = TIM11;

  /* Initialize TIMx peripheral as follow:
   * Period = [(TIM11CLK/1000) - 1]. to have a (1/1000) s time base.
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
   * Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
   * ClockDivision = 0
   * Counter direction = Up
   */
<<<<<<< HEAD
  htim5.Init.Period = (1000000U / 1000U) - 1U;
  htim5.Init.Prescaler = uwPrescalerValue;
  htim5.Init.ClockDivision = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  status = HAL_TIM_Base_Init(&htim5);
  if (status == HAL_OK)
  {

    /* Start the TIM time Base generation in interrupt mode */
    status = HAL_TIM_Base_Start_IT(&htim5);
    if (status == HAL_OK)
    {
    /* Enable the TIM5 global Interrupt */
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
=======
  htim11.Init.Period = (1000000U / 1000U) - 1U;
  htim11.Init.Prescaler = uwPrescalerValue;
  htim11.Init.ClockDivision = 0;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  status = HAL_TIM_Base_Init(&htim11);
  if (status == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    status = HAL_TIM_Base_Start_IT(&htim11);
    if (status == HAL_OK)
    {
    /* Enable the TIM11 global Interrupt */
        HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
      /* Configure the SysTick IRQ priority */
      if (TickPriority < (1UL << __NVIC_PRIO_BITS))
      {
        /* Configure the TIM IRQ priority */
<<<<<<< HEAD
        HAL_NVIC_SetPriority(TIM5_IRQn, TickPriority, 0U);
=======
        HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, TickPriority, 0U);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
        uwTickPrio = TickPriority;
      }
      else
      {
        status = HAL_ERROR;
      }
    }
  }

 /* Return function status */
  return status;
}

/**
  * @brief  Suspend Tick increment.
<<<<<<< HEAD
  * @note   Disable the tick increment by disabling TIM5 update interrupt.
=======
  * @note   Disable the tick increment by disabling TIM11 update interrupt.
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
<<<<<<< HEAD
  /* Disable TIM5 update Interrupt */
  __HAL_TIM_DISABLE_IT(&htim5, TIM_IT_UPDATE);
=======
  /* Disable TIM11 update Interrupt */
  __HAL_TIM_DISABLE_IT(&htim11, TIM_IT_UPDATE);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
}

/**
  * @brief  Resume Tick increment.
<<<<<<< HEAD
  * @note   Enable the tick increment by Enabling TIM5 update interrupt.
=======
  * @note   Enable the tick increment by Enabling TIM11 update interrupt.
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
<<<<<<< HEAD
  /* Enable TIM5 Update interrupt */
  __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
=======
  /* Enable TIM11 Update interrupt */
  __HAL_TIM_ENABLE_IT(&htim11, TIM_IT_UPDATE);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
}

