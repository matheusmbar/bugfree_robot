/*
 * uart_driver.h
 *
 *  Created on: 8 de abr de 2020
 *      Author: matheus
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "retcode.h"
#include <usart.h>

typedef struct {
    UART_HandleTypeDef* huart;
    uint8_t* buffer;
    uint8_t  head_pos;
    uint8_t  tail_pos;
    uint16_t buffer_size;
    uint16_t bytes_available;

    uint8_t  last_pos;
}uart_buffer_t;

retcode_t uartDriver_init(uart_buffer_t * uart_buffer);

void uartDriver_lineIdle(uart_buffer_t * buffer, uint32_t dma_pos);

uint16_t uartDriver_getAvailable(uart_buffer_t * buffer);

void uartDriver_free (uart_buffer_t * uart_buffer, uint16_t bytes_to_free);

#endif /* UART_DRIVER_H_ */
