/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bugfree/utils/log/log.h"
#include "bugfree/drivers/io/io.h"
#include <strings.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//RX buffer must be bigger than bigger message length expected to receive
//its size must consider the relation between the speed that data is received
//and the time it takes to process this data
#define RX_BUFFER_LEN	(100)
uint8_t rx_buffer[RX_BUFFER_LEN];
uint8_t idle_line_counter = 0;
uint32_t bytes_received = 0;
uint8_t same_pos_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void uart_proccess_data (uint8_t* rx_start, uint8_t len){
    HAL_UART_Transmit(&huart4, rx_start, len, HAL_MAX_DELAY);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if (io_getLevel(IO_LED_GREEN) == IO_HIGH){
		io_setLow(IO_LED_GREEN);
	}
	else{
		io_setHigh(IO_LED_GREEN);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (io_getLevel(IO_LED_RED) == IO_HIGH){
		io_setLow(IO_LED_RED);
	}
	else{
		io_setHigh(IO_LED_RED);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	if (io_getLevel(IO_LED_BLUE) == IO_HIGH){
		io_setLow(IO_LED_BLUE);
	}
	else{
		io_setHigh(IO_LED_BLUE);
	}
}

void uart4_lineIdle (void){
	static uint32_t old_pos = 0;

	uint32_t pos = RX_BUFFER_LEN - huart4.hdmarx->Instance->CNDTR;

	if (pos == old_pos){
		same_pos_counter ++;
		return;
	}
	idle_line_counter ++;

	if (pos > old_pos){
		bytes_received = pos - old_pos;
		uart_proccess_data(&rx_buffer[old_pos], bytes_received);
	}
	else{
		bytes_received = RX_BUFFER_LEN - old_pos + pos;
        uart_proccess_data(&rx_buffer[old_pos], RX_BUFFER_LEN - old_pos);
        uart_proccess_data(&rx_buffer[0], pos);
	}
	old_pos = pos;

	if (io_getLevel(IO_LED_YELLOW) == IO_HIGH){
		io_setLow(IO_LED_YELLOW);
	}
	else{
		io_setHigh(IO_LED_YELLOW);
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  log_init(LOG_DEBUG);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    uint8_t msg[30];
    uint8_t len;
    uint8_t counter = 0;


    HAL_UART_Receive_DMA(&huart4, rx_buffer, RX_BUFFER_LEN);

    uint8_t last_idle_line_counter = 0;
    while (1) {
//        if (last_idle_line_counter != idle_line_counter) {
//            last_idle_line_counter = idle_line_counter;
//            len = snprintf(msg, 30, "Hello world\t%d\t%d\t%lu\t%d\r\n", counter, idle_line_counter, bytes_received, same_pos_counter);
//            len = len > 30 ? 30 : len;
//            HAL_UART_Transmit_DMA(&huart4, msg, len);
//            counter++;
//        }

//	 HAL_UART_Receive(&huart4, rx_buffer, 1, 500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART5;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
