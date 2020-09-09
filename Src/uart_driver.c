/*
 * uart_driver.c
 *
 *  Created on: 8 de abr de 2020
 *      Author: matheus
 */


#include "uart_driver.h"

#include <usart.h>

// Private variables and defines

#define RX_BUFFER_LEN   (50)
uint8_t rx_buffer[RX_BUFFER_LEN];
uint8_t idle_line_counter = 0;
uint8_t same_pos_counter = 0;

uint16_t tail;
uint16_t head;
uint16_t bytes_available;

// Private function prototypes
inline void uartDriver_updateAvailable (void);

// HAL UART IRQ Callbacks

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
//    if (io_getLevel(IO_LED_GREEN) == IO_HIGH){
//        io_setLow(IO_LED_GREEN);
//    }
//    else{
//        io_setHigh(IO_LED_GREEN);
//    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//    if (io_getLevel(IO_LED_RED) == IO_HIGH){
//        io_setLow(IO_LED_RED);
//    }
//    else{
//        io_setHigh(IO_LED_RED);
//    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
//    if (io_getLevel(IO_LED_BLUE) == IO_HIGH){
//        io_setLow(IO_LED_BLUE);
//    }
//    else{
//        io_setHigh(IO_LED_BLUE);
//    }
}


// Uart Driver functions
retcode_t uartDriver_init(void){
    tail = 0;
    head = 0;
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, rx_buffer, RX_BUFFER_LEN);
    uartDriver_updateAvailable();
    return RET_OK;
}

void uartDriver_lineIdle4(void){
    static uint32_t old_pos = 0;
    uint16_t bytes_received;

    uint32_t pos = RX_BUFFER_LEN - huart4.hdmarx->Instance->CNDTR;

    if (pos == old_pos){
        same_pos_counter ++;
        return;
    }
    idle_line_counter ++;

    if (pos > old_pos){
        bytes_received = pos - old_pos;
    }
    else{
        bytes_received = RX_BUFFER_LEN - old_pos + pos;
    }
    old_pos = pos;
    head = pos;

    uartDriver_updateAvailable();
}

void uartDriver_getAvailable(uart_buffer_t * buffer){
    buffer->buffer = rx_buffer;
    buffer->head = &rx_buffer[head];
    buffer->tail = &rx_buffer[tail];
    buffer->head_pos = head;
    buffer->tail_pos = tail;
    buffer->buffer_size = RX_BUFFER_LEN;
    buffer->bytes_available = bytes_available;
}

void uartDriver_free (uint16_t bytes_to_free){
    if (bytes_to_free >= bytes_available){
        tail = head;
    }
    else{
        tail = (tail + bytes_to_free) % RX_BUFFER_LEN;
    }
    uartDriver_updateAvailable();
}

// Private functions
void uartDriver_updateAvailable (void){
    if (head < tail){
        bytes_available = tail - head;
    }
    else if (tail < head){
        bytes_available = RX_BUFFER_LEN - head + tail;
    }
    else{
        bytes_available = RX_BUFFER_LEN;
    }
}




