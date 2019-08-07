
#ifndef DRIVERS_IO_H
#define DRIVERS_IO_H

#include <stdint.h>

/* Data typedefs--------------------------------------------------------------*/

typedef enum{
    IO_INVALID = 0,
    //LEDs
    IO_LED_BLUE, IO_LED_GREEN, IO_LED_RED, IO_LED_YELLOW, 
}io_dev_t;

/* Function Prototypes--------------------------------------------------------*/


/* Sets a GPIO pin to HIGH level */
void io_setHigh (io_dev_t device);

/* Sets a GPIO pin to LOW level */
void io_setLow  (io_dev_t device);

#endif