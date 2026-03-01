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

TX_QUEUE Queue1;

TX_THREAD SenderTask1;
TX_THREAD SenderTask2;
TX_THREAD ReceiverTask1;

uint8_t rx_data = 0;

ULONG queue_storage[5];
UINT queue_status;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	int data = 333;
	char *str;

	if(huart->Instance == USART2)
	{
		if(rx_data == 'r')
		{
			if(tx_queue_front_send(&Queue1, &data, TX_NO_WAIT) == TX_SUCCESS)
			{
				str = "\r\nSending Data To Queue Via ISR\r\n";
				HAL_UART_Transmit(huart, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);
			}
			else
			{
                str = "\r\nError: Queue Is Currently Full\r\n";
                HAL_UART_Transmit(huart, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);
			}
		}
	}

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void SenderTask1_Init(ULONG thread_input);
void SenderTask2_Init(ULONG thread_input);
void ReceiverTask1_Init(ULONG thread_input);

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
  queue_status = tx_queue_create(&Queue1, "Queue 1", 1, queue_storage, sizeof(queue_storage));

	if(queue_status != TX_SUCCESS)
	{
		char *str = "\r\n\Error: Cannot Create Integer Queue/r/n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), MAX_UART_WAIT_TIME);
	}
	else
	{
		char *str = "\r\nInteger Queue Created Successfully\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), MAX_UART_WAIT_TIME);
	}

  tx_thread_create(&SenderTask1, "SenderTask1", SenderTask1_Init, 0, threadstack1, sizeof(threadstack1), 1, 1, 0, TX_AUTO_START);
  tx_thread_create(&SenderTask2, "SenderTask2", SenderTask2_Init, 0, threadstack2, sizeof(threadstack2), 2, 2, 0, TX_AUTO_START);
  tx_thread_create(&ReceiverTask1, "ReceiverTask1", ReceiverTask1_Init, 0, threadstack3, sizeof(threadstack3), 3, 3, 0, TX_AUTO_START);

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

	HAL_UART_Receive_IT(&huart2, &rx_data, 1);

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

void SenderTask1_Init(ULONG thread_input)
{
	int data = 222;

	while(1)
	{
		char *str = "\r\n\nEntered Sender Task 1, Sending Data To Queue\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);

//		if(tx_queue_send(&Queue1, &data, TX_WAIT_FOREVER) == TX_SUCCESS)
//		{
//			char *str2 = "\r\nSuccessfully Sent Data To Queue\r\n";
//			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
//		}

		if(tx_queue_send(&Queue1, &data, TX_NO_WAIT) == TX_SUCCESS)
		{
			char *str2 = "\r\nSuccessfully Sent Data To Queue\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
		}

		char *str3 = "\r\nLeaving Sender Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), MAX_UART_WAIT_TIME);

		tx_thread_sleep(200);
	}
}

void SenderTask2_Init(ULONG thread_input)
{
	int data = 111;

	while(1)
	{
		char *str = "\r\n\nEntered Sender Task 2, Sending Data To Queue\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);

//		if(tx_queue_send(&Queue1, &data, TX_WAIT_FOREVER) == TX_SUCCESS)
//		{
//			char *str2 = "\r\nSuccessfully Sent Data To Queue\r\n";
//			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
//		}

		if(tx_queue_send(&Queue1, &data, TX_NO_WAIT) == TX_SUCCESS)
		{
			char *str2 = "\r\nSuccessfully Sent Data To Queue\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
		}

		char *str3 = "\r\nLeaving Sender Task 2\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), MAX_UART_WAIT_TIME);

		tx_thread_sleep(200);
	}
}

void ReceiverTask1_Init(ULONG thread_input)
{
	int received = 0;

	while(1)
	{
		uint8_t RxMsg[50];
        char *str = "\r\n\nEntered Receiver Task 1, Receiving Data From Queue\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), MAX_UART_WAIT_TIME);

        if(tx_queue_receive(&Queue1, &received, TX_WAIT_FOREVER) != TX_SUCCESS)
        {
			char *str2 = "\r\nError: Failed To Receive Data From Queue\r\n";
			HAL_UART_Transmit(&huart2, (uint8_t *)str2, strlen(str2), MAX_UART_WAIT_TIME);
        }
        else
        {
			int length = sprintf((char*)RxMsg, "\r\nSuccessfully Received Data From Queue: %d\r\n", received);
			HAL_UART_Transmit(&huart2, RxMsg, length, MAX_UART_WAIT_TIME);
        }

//        if(tx_queue_receive(&Queue1, &received, TX_WAIT_FOREVER) == TX_SUCCESS)
//        {
//			int length = sprintf((char*)RxMsg, "\r\nSuccessfully Received Data From Queue: %d\r\n", received);
//			HAL_UART_Transmit(&huart2, RxMsg, length, MAX_UART_WAIT_TIME);
//        }

		char *str3 = "\r\nLeaving Receiver Task 1\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)str3, strlen(str3), MAX_UART_WAIT_TIME);

		tx_thread_sleep(500);
	}
}

/* USER CODE END 1 */
