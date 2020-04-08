#include "io.h"

#include "main.h"
#include "stm32f3xx_hal_gpio.h"

/* Private Data typedefs------------------------------------------------------*/

typedef struct{
    uint16_t pin;
    GPIO_TypeDef* port;
}io_gpio_t;

/* Private Variables ---------------------------------------------------------*/

static const io_gpio_t gpio_array [] = {
    [IO_LED_GREEN]  = {LED_GREEN_Pin,  LED_GREEN_GPIO_Port},
    [IO_LED_BLUE]   = {LED_BLUE_Pin,   LED_BLUE_GPIO_Port},
    [IO_LED_YELLOW] = {LED_YELLOW_Pin, LED_YELLOW_GPIO_Port},
    [IO_LED_RED]    = {LED_RED_Pin,    LED_RED_GPIO_Port},
};

/* Public Functions ----------------------------------------------------------*/

/* Sets a GPIO pin to HIGH level */
void io_setHigh (io_dev_t device){
    HAL_GPIO_WritePin(gpio_array[device].port, 
                        gpio_array[device].pin, GPIO_PIN_SET);
}

/* Sets a GPIO pin to LOW level */
void io_setLow  (io_dev_t device){
    HAL_GPIO_WritePin(gpio_array[device].port, 
                        gpio_array[device].pin, GPIO_PIN_RESET);
}

/* Gets a GPIO pin level */
io_status_t io_getLevel (io_dev_t device){
	if (HAL_GPIO_ReadPin(gpio_array[device].port, gpio_array[device].pin) == GPIO_PIN_SET){
		return IO_HIGH;
	}
	return IO_LOW;
}
