/*
 * uart_driver.h
 *
 *  Created on: 8 de abr de 2020
 *      Author: matheus
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "retcode.h"

typedef struct {
    uint8_t  head_pos;
    uint8_t  tail_pos;
    uint8_t* head;
    uint8_t* tail;
    uint8_t* buffer;
    uint16_t buffer_size;
    uint16_t bytes_available;
}uart_buffer_t;

retcode_t uartDriver_init(void);

void uartDriver_lineIdle4(void);

void uartDriver_getAvailable(uart_buffer_t * buffer);

void uartDriver_free (uint16_t bytes_to_free);

#endif /* UART_DRIVER_H_ */
