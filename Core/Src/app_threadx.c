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
uint8_t threadstack4[STACK_SIZE];

TX_SEMAPHORE CountSem1;

TX_THREAD Task1;
TX_THREAD Task2;
TX_THREAD Task3;
TX_THREAD Task4;

int resource[3] = {111, 222, 333};
int indx = 0;

uint8_t rx_data = 0;

UINT sem_status;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		if(rx_data == 'r')
		{
			// releases the semaphore tokens
			tx_semaphore_ceiling_put(&CountSem1, 3);
			tx_semaphore_ceiling_put(&CountSem1, 3);
			tx_semaphore_ceiling_put(&CountSem1, 3);

//			tx_semaphore_put(&CountSem1);
//			tx_semaphore_put(&CountSem1);
//			tx_semaphore_put(&CountSem1);
		}
	}

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Task1_Init(ULONG thread_input);
void Task2_Init(ULONG thread_input);
void Task3_Init(ULONG thread_input);
void Task4_Init(ULONG thread_input);

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
  sem_status = tx_semaphore_create(&CountSem1, "CountSem1", 0);

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

  tx_thread_create(&Task1, "Task 1", Task1_Init, 0, threadstack1, sizeof(threadstack1), 1, 1, 0, TX_AUTO_START);
  tx_thread_create(&Task2, "Task 2", Task2_Init, 0, threadstack2, sizeof(threadstack2), 2, 2, 0, TX_AUTO_START);
  tx_thread_create(&Task3, "Task 3", Task3_Init, 0, threadstack3, sizeof(threadstack3), 3, 3, 0, TX_AUTO_START);
  tx_thread_create(&Task4, "Task 4", Task4_Init, 0, threadstack4, sizeof(threadstack4), 4, 4, 0, TX_AUTO_START);

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

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

void Task1_Init(ULONG thread_input)
{
	char sresource[3];
	ULONG semcount;
	char tokenNum[2];

	// releases 3 semaphore tokens
	tx_semaphore_put(&CountSem1);
	tx_semaphore_put(&CountSem1);
	tx_semaphore_put(&CountSem1);

	while(1)
	{
		char str[150];
		strcpy(str, "\r\n\nEntered Task 1, Waiting For Semaphore\r\n");

		// returns the number of available semaphore tokens
		tx_semaphore_info_get(&CountSem1, TX_NULL, &semcount, TX_NULL, TX_NULL, TX_NULL);

		itoa(semcount, tokenNum, 10);
		strcat(str, "\r\nTokens Available: ");
		strcat(str, tokenNum);
		strcat(str, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		tx_semaphore_get(&CountSem1, TX_WAIT_FOREVER);

		itoa(resource[indx], sresource, 10);
		strcpy(str, "\r\nLeaving Task 1: ");
		strcat(str, sresource);
		strcat(str, "\r\n\nNot Releasing Semaphore\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		indx++;

		if(indx > 2)
			indx = 0;

		tx_thread_sleep(300);
	}
}

void Task2_Init(ULONG thread_input)
{
	char sresource[3];
	ULONG semcount;
	char tokenNum[2];

	while(1)
	{
		char str[150];
		strcpy(str, "\r\n\nEntered Task 2, Waiting For Semaphore\r\n");

		// returns the number of available semaphore tokens
		tx_semaphore_info_get(&CountSem1, TX_NULL, &semcount, TX_NULL, TX_NULL, TX_NULL);

		itoa(semcount, tokenNum, 10);
		strcat(str, "\r\nTokens Available: ");
		strcat(str, tokenNum);
		strcat(str, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		tx_semaphore_get(&CountSem1, TX_WAIT_FOREVER);

		itoa(resource[indx], sresource, 10);
		strcpy(str, "\r\nLeaving Task 2: ");
		strcat(str, sresource);
		strcat(str, "\r\n\nNot Releasing Semaphore\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		indx++;

		if(indx > 2)
			indx = 0;

		tx_thread_sleep(200);
	}
}

void Task3_Init(ULONG thread_input)
{
	char sresource[3];
	ULONG semcount;
	char tokenNum[2];

	while(1)
	{
		char str[150];
		strcpy(str, "\r\n\nEntered Task 3, Waiting For Semaphore\r\n");

		// returns the number of available semaphore tokens
		tx_semaphore_info_get(&CountSem1, TX_NULL, &semcount, TX_NULL, TX_NULL, TX_NULL);

		itoa(semcount, tokenNum, 10);
		strcat(str, "\r\nTokens Available: ");
		strcat(str, tokenNum);
		strcat(str, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		tx_semaphore_get(&CountSem1, TX_WAIT_FOREVER);

		itoa(resource[indx], sresource, 10);
		strcpy(str, "\r\nLeaving Task 3: ");
		strcat(str, sresource);
		strcat(str, "\r\n\nNot Releasing Semaphore\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		indx++;

		if(indx > 2)
			indx = 0;

		tx_thread_sleep(100);
	}
}

void Task4_Init(ULONG thread_input)
{
	char sresource[3];
	ULONG semcount;
	char tokenNum[2];

	while(1)
	{
		char str[150];
		strcpy(str, "\r\n\nEntered Task 4, Waiting For Semaphore\r\n");

		// returns the number of available semaphore tokens
		tx_semaphore_info_get(&CountSem1, TX_NULL, &semcount, TX_NULL, TX_NULL, TX_NULL);

		itoa(semcount, tokenNum, 10);
		strcat(str, "\r\nTokens Available: ");
		strcat(str, tokenNum);
		strcat(str, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		tx_semaphore_get(&CountSem1, TX_WAIT_FOREVER);

		itoa(resource[indx], sresource, 10);
		strcpy(str, "\r\nLeaving Task 4: ");
		strcat(str, sresource);
		strcat(str, "\r\n\nNot Releasing Semaphore\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 2000);

		indx++;

		if(indx > 2)
			indx = 0;

		tx_thread_sleep(300);
	}
}

/* USER CODE END 1 */
