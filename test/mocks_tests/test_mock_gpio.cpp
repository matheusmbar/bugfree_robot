#include "CppUTest/TestHarness.h"
#include <CppUTestExt/MockSupport_c.h>

extern "C"{
    #include <stm32f3xx_hal_gpio.h>
}

TEST_GROUP(mock_gpio){
        GPIO_TypeDef  gpio;
        GPIO_TypeDef* test_gpio;
        GPIO_TypeDef  gpio2;
        GPIO_TypeDef* test_gpio2;
    void setup (){
        test_gpio = &gpio;
        test_gpio2 = &gpio2;

    }
   void teardown (){
        mock_c()->checkExpectations();
        mock_c()->clear();
   }
};

TEST (mock_gpio, write_pin){

    mock_c()->expectOneCall("HAL_GPIO_WritePin")
        ->withPointerParameters    ("GPIOx",    test_gpio)
        ->withUnsignedIntParameters("GPIO_Pin", GPIO_PIN_1)
        ->withUnsignedIntParameters("PinState", GPIO_PIN_SET);

    HAL_GPIO_WritePin (test_gpio, GPIO_PIN_1, GPIO_PIN_SET);

    mock_c()->expectOneCall("HAL_GPIO_WritePin")
        ->withPointerParameters    ("GPIOx",    test_gpio2)
        ->withUnsignedIntParameters("GPIO_Pin", GPIO_PIN_3)
        ->withUnsignedIntParameters("PinState", GPIO_PIN_RESET);

    HAL_GPIO_WritePin (test_gpio2, GPIO_PIN_3, GPIO_PIN_RESET);
}

TEST (mock_gpio, read_pin){
    GPIO_PinState pin_state = GPIO_PIN_RESET;
    mock_c()->expectOneCall("HAL_GPIO_ReadPin")
        ->withPointerParameters    ("GPIOx",    test_gpio)
        ->withUnsignedIntParameters("GPIO_Pin", GPIO_PIN_3)
        ->andReturnUnsignedIntValue (GPIO_PIN_SET);
    pin_state = HAL_GPIO_ReadPin(test_gpio, GPIO_PIN_3);
    CHECK_EQUAL (GPIO_PIN_SET, pin_state);

    mock_c()->expectOneCall("HAL_GPIO_ReadPin")
        ->withPointerParameters    ("GPIOx",    test_gpio2)
        ->withUnsignedIntParameters("GPIO_Pin", GPIO_PIN_7)
        ->andReturnUnsignedIntValue (GPIO_PIN_RESET);
    pin_state = HAL_GPIO_ReadPin(test_gpio2, GPIO_PIN_7);
    CHECK_EQUAL (GPIO_PIN_RESET, pin_state);
}
