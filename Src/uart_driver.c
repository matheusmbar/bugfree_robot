/*
 * uart_driver.c
 *
 *  Created on: 8 de abr de 2020
 *      Author: matheus
 */


#include "uart_driver.h"


// Private variables and defines

#define RX_BUFFER_LEN   (50)
uint8_t idle_line_counter = 0;
uint8_t same_pos_counter = 0;


// Private function prototypes
inline void uartDriver_updateReceived (uart_buffer_t * buffer);

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
retcode_t uartDriver_init(uart_buffer_t * buffer){
    buffer->tail_pos = 0;
    buffer->tail_pos = 0;
    buffer->last_pos = 0;

    __HAL_UART_ENABLE_IT(buffer->huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(buffer->huart, buffer->buffer, buffer->buffer_size);
    uartDriver_updateReceived(buffer);
    return RET_OK;
}

void uartDriver_lineIdle(uart_buffer_t * buffer, uint32_t dma_pos){
    uint32_t pos = buffer->buffer_size - dma_pos;

    if (pos == buffer->head_pos){
        return;
    }

    buffer->head_pos = pos;
    uartDriver_updateReceived(buffer);
}

uint16_t uartDriver_getFreeBytes(uart_buffer_t * buffer){
    return buffer->buffer_size - buffer->bytes_received;
}

uint16_t uartDriver_getAvailableRX(uart_buffer_t * buffer){
    return buffer->bytes_received;
}

void uartDriver_free (uart_buffer_t * buffer, uint16_t bytes_to_free){
    if (bytes_to_free >= buffer->bytes_received){
        buffer->tail_pos = buffer->head_pos;
    }
    else{
        buffer->tail_pos = (buffer->tail_pos + bytes_to_free) % buffer->buffer_size;
    }
    uartDriver_updateReceived(buffer);
}

// Private functions
void uartDriver_updateReceived (uart_buffer_t * buffer){
    if (buffer->tail_pos < buffer->head_pos){
        buffer->bytes_received = buffer->head_pos - buffer->tail_pos;
    }
    else if (buffer->head_pos < buffer->tail_pos){
        buffer->bytes_received = buffer->buffer_size - buffer->head_pos + buffer->tail_pos;
    }
    else{
        buffer->bytes_received = 0;
    }
}




