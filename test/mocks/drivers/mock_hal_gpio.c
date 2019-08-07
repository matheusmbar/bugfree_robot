#include <CppUTestExt/MockSupport_c.h>
#include <stm32f3xx_hal_gpio.h>

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
    return 0;
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState){
    mock_c()->actualCall("HAL_GPIO_WritePin")
            ->withPointerParameters("GPIOx", GPIOx)
            ->withUnsignedIntParameters("GPIO_Pin", GPIO_Pin)
            ->withUnsignedIntParameters("PinState", PinState);
}
