/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "tx_api.h"

#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STACK_SIZE  1024

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t threadstack1[STACK_SIZE];
uint8_t threadstack2[STACK_SIZE];
uint8_t threadstack3[STACK_SIZE];

TX_TIMER periodTIM;
TX_TIMER osTIM;

TX_THREAD uartTask;
TX_THREAD ledTask;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void UART_Task(ULONG thread_input);
void LED_Task(ULONG thread_input);

void PT_Callback(ULONG input);
void OS_Callback(ULONG input);

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  tx_timer_create(&periodTIM, "Period Timer", PT_Callback, 1, 100, 100, TX_AUTO_ACTIVATE);
  tx_timer_create(&osTIM, "One Shot Timer", OS_Callback, 2, 400, 0, TX_NO_ACTIVATE);

  tx_thread_create(&uartTask, "UART Task", UART_Task, 0, threadstack1, sizeof(threadstack1), 1, 1, 0, TX_AUTO_START);
  tx_thread_create(&ledTask, "LED Task", LED_Task, 0, threadstack2, sizeof(threadstack2), 1, 1, 0, TX_AUTO_START);

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */
	char *border = "\r\n------------------------------------------------\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t *)border, strlen(border), 2000);

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void PT_Callback(ULONG input)
{
	char *str = "\r\nSending From Period TIM\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);
}

void OS_Callback(ULONG input)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

//	char *str = "\r\nSending From One Shot TIM\r\n";
//	HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);
}

void UART_Task(ULONG thread_input)
{
	tx_timer_activate(&periodTIM);

	while(1)
	{
		char *str = "\r\nSending From UART Thread\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);
		tx_thread_sleep(200);
	}
}

void LED_Task(ULONG thread_input)
{
	while(1)
	{
		// NOTE: PC13 is pulled HIGH (1) by default
		// So this if-statement is checking if B1 has been pressed

		if(!(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			tx_timer_change(&osTIM, 400, 0);
			tx_timer_activate(&osTIM);
		}

		tx_thread_sleep(1);
	}
}

/* USER CODE END 1 */
