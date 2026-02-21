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
uint8_t threadstack1[STACK_SIZE];
uint8_t threadstack2[STACK_SIZE];
uint8_t threadstack3[STACK_SIZE];

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t task1[] = "\r\nRunning Task 1\r\n";
uint8_t task2[] = "\r\n\r\nRunning Task 2\r\n";
uint8_t task3[] = "\r\nRunning Task 3\r\n";
uint8_t suspend1[] = "\r\nSuspending Task 1\r\n";
uint8_t resume1[] = "\r\nResuming Task 1\r\n";
uint8_t delay2[] = "\r\nDelaying Task 2\r\n\r\n";
uint8_t terminate2[] = "\r\nTerminating Task 2\r\n\r\n";
//uint8_t task2[] = "\r\nTask 2 Initiated\r\n";

uint8_t count2 = 0;
uint8_t RxMsg[50];

TX_THREAD Thread1;
TX_THREAD Thread2;
TX_THREAD Thread3;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Thread1_Entry(ULONG thread_input);
void Thread2_Entry(ULONG thread_input);
void Thread3_Entry(ULONG thread_input);

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
  tx_thread_create(&Thread1, "Thread 1", Thread1_Entry, 0, threadstack1, sizeof(threadstack1), 2, 2, 0, TX_AUTO_START);
  tx_thread_create(&Thread2, "Thread 2", Thread2_Entry, 0, threadstack2, sizeof(threadstack2), 1, 1, 0, TX_AUTO_START);
  tx_thread_create(&Thread3, "Thread 3", Thread3_Entry, 0, threadstack3, sizeof(threadstack3), 3, 3, 0, TX_AUTO_START);

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
void Thread1_Entry(ULONG thread_input)
{
	while(1)
	{
		HAL_UART_Transmit(&huart2, task1, sizeof(task1), 2000);
		tx_thread_sleep(100);
	}
}

void Thread2_Entry(ULONG thread_input)
{
	while(1)
	{
		  int length = sprintf((char*)RxMsg, "\r\n\r\nRunning Task 2: %d\r\n", count2);
		  HAL_UART_Transmit(&huart2, RxMsg, length, 2000);
		  tx_thread_sleep(100);

		  if(count2 == 5)
		  {
//			  printf("\r\nSuspending Task 1\r\n");
			  HAL_UART_Transmit(&huart2, suspend1, sizeof(suspend1), 2000);
			  tx_thread_suspend(&Thread1);
		  }

		  if(count2 == 10)
		  {
//		      printf("\r\nResuming Task 1\r\n");
			  HAL_UART_Transmit(&huart2, resume1, sizeof(resume1), 2000);
			  tx_thread_resume(&Thread1);
		  }

		  if(count2 == 15)
		  {
//		      printf("\r\nDelaying Task 2\r\n\r\n");
			  HAL_UART_Transmit(&huart2, delay2, sizeof(delay2), 2000);

			  // Grabs current tick count
			  ULONG mark = tx_time_get();

			  // Delays task 2 for 3 seconds
			  while(tx_time_get() < mark + 300)
			  {
				  tx_thread_sleep(1);
			  }
		  }

		  if(count2 == 20)
		  {
	//		  printf("\r\nTerminating Task 2\r\n");
			  HAL_UART_Transmit(&huart2, terminate2, sizeof(terminate2), 2000);
			  tx_thread_terminate(&Thread2);
		  }

		  count2++;
	}
}

void Thread3_Entry(ULONG thread_input)
{
	while(1)
	{
		HAL_UART_Transmit(&huart2, task3, sizeof(task3), 2000);
		tx_thread_sleep(100);
	}
}

/* USER CODE END 1 */
