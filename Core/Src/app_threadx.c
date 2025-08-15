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

TX_SEMAPHORE BinSem1;

TX_THREAD NormalTask;
TX_THREAD HighTask;
TX_THREAD LowTask;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void StartNormalTask(ULONG thread_input);
void StartHighTask(ULONG thread_input);
void StartLowTask(ULONG thread_input);

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
  tx_semaphore_create(&BinSem1, "BinSem1", 1);

  tx_thread_create(&NormalTask, "Normal Task", StartNormalTask, 0, threadstack1, sizeof(threadstack1), 2, 2, 0, TX_AUTO_START);
  tx_thread_create(&HighTask, "High Task", StartHighTask, 0, threadstack2, sizeof(threadstack2), 1, 1, 0, TX_AUTO_START);
  tx_thread_create(&LowTask, "Low Task", StartLowTask, 0, threadstack3, sizeof(threadstack3), 3, 3, 0, TX_AUTO_START);

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

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

void StartNormalTask(ULONG thread_input)
{
	while(1)
	{
		char *normal1 = "\r\nEntered Normal Task, Waiting For Semaphore\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)normal1, strlen(normal1), 2000);

		tx_semaphore_get(&BinSem1, TX_WAIT_FOREVER);

		char *normal3 = "\r\nSemaphore Obtained By Normal Task\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)normal3, strlen(normal3), 2000);

		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));

		char *normal2 = "\r\nLeaving Normal Task, Releasing Semaphore\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)normal2, strlen(normal2), 2000);

		tx_semaphore_ceiling_put(&BinSem1, 1);
		tx_thread_sleep(100);
	}
}

void StartHighTask(ULONG thread_input)
{
	while(1)
	{
		char *high1 = "\r\n\r\nEntered High Task, Waiting For Semaphore\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)high1, strlen(high1), 2000);

		tx_semaphore_get(&BinSem1, TX_WAIT_FOREVER);

		char *high3 = "\r\nSemaphore Obtained By High Task\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)high3, strlen(high3), 2000);

		char *high2 = "\r\nLeaving High Task, Releasing Semaphore\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)high2, strlen(high2), 2000);

		tx_semaphore_ceiling_put(&BinSem1, 1);
		tx_thread_sleep(100);
	}
}

void StartLowTask(ULONG thread_input)
{
	while(1)
	{
		char *low1 = "\r\nEntered Low Task\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)low1, strlen(low1), 2000);

		char *low2 = "\r\nLeaving Low Task\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)low2, strlen(low2), 2000);

		tx_thread_sleep(100);
	}
}

/* USER CODE END 1 */
