/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

<<<<<<< HEAD
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SCREEN_UP_RELAY_Pin|SCREEN_DOWN_RELAY_Pin|CURTAIN_OPEN_RELAY_Pin|CURTAIN_CLOSE_RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LIGHT_RELAY_GPIO_Port, LIGHT_RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SCREEN_UP_RELAY_Pin SCREEN_DOWN_RELAY_Pin CURTAIN_OPEN_RELAY_Pin CURTAIN_CLOSE_RELAY_Pin */
  GPIO_InitStruct.Pin = SCREEN_UP_RELAY_Pin|SCREEN_DOWN_RELAY_Pin|CURTAIN_OPEN_RELAY_Pin|CURTAIN_CLOSE_RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LIGHT_RELAY_Pin */
  GPIO_InitStruct.Pin = LIGHT_RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LIGHT_RELAY_GPIO_Port, &GPIO_InitStruct);
=======
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, RELAY_1_LIGHT_Pin|RELAY_2_CURTAIN_OPEN_Pin|RELAY_3_CURTAIN_CLOSE_Pin|RELAY_4_SCREEN_UP_Pin);

  /**/
  LL_GPIO_ResetOutputPin(RELAY_1_LIGHTA15_GPIO_Port, RELAY_1_LIGHTA15_Pin);

  /**/
  GPIO_InitStruct.Pin = RELAY_1_LIGHT_Pin|RELAY_2_CURTAIN_OPEN_Pin|RELAY_3_CURTAIN_CLOSE_Pin|RELAY_4_SCREEN_UP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RELAY_1_LIGHTA15_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RELAY_1_LIGHTA15_GPIO_Port, &GPIO_InitStruct);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
