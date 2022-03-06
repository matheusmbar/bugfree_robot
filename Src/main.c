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
#include <stdio.h>
#include "uart_driver.h"
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
uart_buffer_t buff_u4;

int counter = 0;

//RX buffer must be bigger than bigger message length expected to receive
//its size must consider the relation between the speed that data is received
//and the time it takes to process this data


int __io_putchar(int ch){
   ITM_SendChar(ch);
   return(ch);
}

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

  log_init(LOG_DEBUG);


  //setup uart driver parameters and call Init
  buff_u4.buffer_size = 180;
  uint8_t buffer_u4_rx[buff_u4.buffer_size];
  buff_u4.buffer = buffer_u4_rx;
  buff_u4.huart = &huart4;
  uartDriver_init(&buff_u4);

  for (uint16_t i = 0; i < buff_u4.buffer_size; i++){
      buffer_u4_rx[i]= '*';
  }

  HAL_Delay(500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    uint8_t msg[200];
    uint8_t len;


    uint16_t last_bytes_received = 1;
    uint16_t bytes_received = 0;
    uint16_t bytes_free = 0;


    while (1) {
        //testing SWO debug print through ST-Link
//        printf("teste %d\n", counter);

        bytes_received = uartDriver_getAvailableRX(&buff_u4);

        if (bytes_received != last_bytes_received){
            bytes_free = uartDriver_getFreeBytes(&buff_u4);
            last_bytes_received = bytes_received;


            len = snprintf((char*) msg, 100, "\r\nUart buffer\r\n\nHead %d\n\rTail %d\n\rbytes_received %u\n\rbytes_used %u\r\n\n",
                            (int)buff_u4.head_pos, (int)buff_u4.tail_pos, bytes_received, bytes_free);
            len = len > 100 ? 100 : len;

            //print buffer contents
            HAL_UART_Transmit(&huart4, msg, len, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart4, buff_u4.buffer, buff_u4.buffer_size, HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart4, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);

            //print header and tail positions right under buffer contents
            for (uint8_t i = 0; i < buff_u4.buffer_size; i++){
                uint8_t c = '_';
                if (i == buff_u4.head_pos){
                    if(buff_u4.head_pos == buff_u4.tail_pos){
                        c = 'x';
                    }
                    else{
                        c = 'H';
                    }
                }
                else if (i == buff_u4.tail_pos){
                    c = 'T';
                }

                HAL_UART_Transmit(&huart4, &c, 1, HAL_MAX_DELAY);
            }

            //deal with buffer content if it has more than 40 bytes
            if (bytes_received > 40){
                uartDriver_free(&buff_u4, bytes_received);
                len = snprintf((char*) msg, 100, "\t\tFREE %d bytes", bytes_received);
                HAL_UART_Transmit(&huart4, msg, len, HAL_MAX_DELAY);
            }
            HAL_UART_Transmit(&huart4, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);

            counter ++;
            HAL_Delay(100);
        }

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
