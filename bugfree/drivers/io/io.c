#include "io.h"

#include "main.h"
#include "stm32f3xx_hal_gpio.h"

/* Private Data typedefs------------------------------------------------------*/

typedef struct{
    uint16_t pin;
    GPIO_TypeDef* port;
}io_gpio_t;

/* Private Variables ---------------------------------------------------------*/

/* Public Functions ----------------------------------------------------------*/

/* Sets a GPIO pin to HIGH level */
void io_setHigh (io_dev_t device){

}

/* Sets a GPIO pin to LOW level */
void io_setLow  (io_dev_t device){

}