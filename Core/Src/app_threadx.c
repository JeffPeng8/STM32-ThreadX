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
#define TASK1_READY      (1 << 0)
#define TASK2_READY      (1 << 1)

#define TASKSET1_READY    (TASK1_READY | TASK2_READY)

#define STACK_SIZE             1024
#define MAX_UART_WAIT_TIME     100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t threadstack1[STACK_SIZE];
uint8_t threadstack2[STACK_SIZE];
uint8_t threadstack3[STACK_SIZE];

TX_MUTEX Mutex1;
TX_EVENT_FLAGS_GROUP Event1;

TX_THREAD Task1;
TX_THREAD Task2;
TX_THREAD Task3;

UINT mutex_status;
UINT event_status;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Task1_Init(ULONG thread_input);
void Task2_Init(ULONG thread_input);
void Task3_Init(ULONG thread_input);

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

  mutex_status = tx_mutex_create(&Mutex1, "Mutex1", TX_INHERIT);
  event_status = tx_event_flags_create(&Event1, "Event 1");

  if(mutex_status == TX_SUCCESS)
  {
	  char *str = "\r\nMutex Successfully Created\r\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), MAX_UART_WAIT_TIME);
  }
  else
  {
	  char *str = "\r\nError: Cannot Create Mutex\r\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), MAX_UART_WAIT_TIME);
  }

  if(event_status == TX_SUCCESS)
  {
	  char *str = "\r\nEvent Flags Successfully Created\r\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), MAX_UART_WAIT_TIME);
  }
  else
  {
	  char *str = "\r\nError: Cannot Create Event Flags\r\n";
	  HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), MAX_UART_WAIT_TIME);
  }

  tx_thread_create(&Task1, "Task 1", Task1_Init, 0, threadstack1, sizeof(threadstack1), 1, 1, 0, TX_AUTO_START);
  tx_thread_create(&Task2, "Task 2", Task2_Init, 0, threadstack2, sizeof(threadstack2), 2, 2, 0, TX_AUTO_START);
  tx_thread_create(&Task3, "Task 3", Task3_Init, 0, threadstack3, sizeof(threadstack3), 3, 3, 0, TX_AUTO_START);

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
  HAL_UART_Transmit(&huart2, (uint8_t *)border, strlen(border), MAX_UART_WAIT_TIME);

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

// Prints the UART logs for task 1
void Task1_Init(ULONG thread_input)
{
	while(1)
	{
		tx_mutex_get(&Mutex1, TX_WAIT_FOREVER);

		char *str = "\r\n\nEntered Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);

		char *str2 = "\r\nSetting Bit For Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);

		char *str3 = "\r\nLeaving Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), MAX_UART_WAIT_TIME);

		tx_mutex_put(&Mutex1);
		tx_event_flags_set(&Event1, TASK1_READY, TX_OR);

		tx_thread_sleep(100);
	}
}

// Prints the UART logs for task 2
void Task2_Init(ULONG thread_input)
{
	ULONG bits;

	while(1)
	{
		tx_event_flags_get(&Event1, TASK1_READY, TX_OR_CLEAR, &bits, TX_WAIT_FOREVER);

		tx_mutex_get(&Mutex1, TX_WAIT_FOREVER);

		char *str = "\r\n\nEntered Task 2\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);

		if((bits & TASK1_READY) != 0)
		{
			char *str2 = "\r\nTask 1 Bit Detected, Setting Bit For Task 2\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
		}

		char *str3 = "\r\nLeaving Task 2\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), MAX_UART_WAIT_TIME);

		tx_mutex_put(&Mutex1);
		tx_event_flags_set(&Event1, TASK2_READY, TX_OR);

		tx_thread_sleep(100);
	}
}

// Prints the UART logs for task 3
void Task3_Init(ULONG thread_input)
{
	ULONG bits;

	while(1)
	{
		tx_event_flags_get(&Event1, TASKSET1_READY, TX_AND_CLEAR, &bits, TX_WAIT_FOREVER);

		tx_mutex_get(&Mutex1, TX_WAIT_FOREVER);

		char *str = "\r\n\nEntered Task 3\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);

		if((bits & TASKSET1_READY) == TASKSET1_READY)
		{
			char *str2 = "\r\nBits For Tasks 1 & 2 Detected\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
		}

		char *str3 = "\r\nLeaving Task 3\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), MAX_UART_WAIT_TIME);

		tx_mutex_put(&Mutex1);

		tx_thread_sleep(100);
	}
}

/* USER CODE END 1 */
