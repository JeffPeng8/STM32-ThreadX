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

TX_MUTEX Mutex1;
TX_SEMAPHORE BinSem1;

TX_THREAD Task1;
TX_THREAD Task2;
TX_THREAD Task3;

UINT mutex_status;
UINT sem_status;

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
  sem_status = tx_semaphore_create(&BinSem1, "BinSem1", 1);

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
	HAL_UART_Transmit(&huart2, (uint8_t *)border, strlen(border), 2000);

	if(mutex_status == TX_SUCCESS)
	{
		char *str = "\r\nMutex Successfully Created\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 2000);
	}
	else
	{
		char *str = "\r\nError: Cannot Create Mutex\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 2000);
	}

	if(sem_status == TX_SUCCESS)
	{
		char *str = "\r\nSemaphore Successfully Created\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 2000);
	}
	else
	{
		char *str = "\r\nError: Cannot Create Semaphore\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 2000);
	}

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void Handle_Mutex(char *str)
{
	// acquires the mutex
	tx_mutex_get(&Mutex1, TX_WAIT_FOREVER);

	tx_thread_sleep(500);
	HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

	// releases the mutex
	tx_mutex_put(&Mutex1);
}

// BS: binary semaphore
void Handle_BS(char *str)
{
	// acquires the semaphore
	tx_semaphore_get(&BinSem1, TX_WAIT_FOREVER);

	tx_thread_sleep(500);
	HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

	// releases the semaphore
	tx_semaphore_ceiling_put(&BinSem1, 1);
}

void Task1_Init(ULONG thread_input)
{
	while(1)
	{
		char *str = "\r\n\nEntered Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		char *str2 = "\r\nMutex Obtained By Task 1\r\n";
		Handle_Mutex(str2);

		char *str3 = "\r\nLeaving Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), 2000);

		tx_thread_sleep(75);
	}
}

void Task2_Init(ULONG thread_input)
{
	while(1)
	{
		char *str = "\r\n\nEntered Task 2\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

//		char *str2 = "\r\nMutex Obtained By Task 2\r\n";
//		Handle_Mutex(str2);

		char *str3 = "\r\nLeaving Task 2\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), 2000);

		tx_thread_sleep(200);
	}
}

void Task3_Init(ULONG thread_input)
{
	while(1)
	{
		char *str = "\r\n\nEntered Task 3\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 2000);

		char *str2 = "\r\nMutex Obtained By Task 3\r\n";
		Handle_Mutex(str2);

		char *str3 = "\r\nLeaving Task 3\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), 2000);

		tx_thread_sleep(100);
	}
}

/* USER CODE END 1 */
