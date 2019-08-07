#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport_c.h>

extern "C"{
    #include "bugfree/drivers/io/io.h"
}
#include <main.h>

TEST_GROUP(driver_io){
    void teardown (){
        mock_c()->checkExpectations();
        mock_c()->clear();
    }
};

TEST (driver_io, set_pin_high){
    mock_c()->expectOneCall("HAL_GPIO_WritePin")
            ->withPointerParameters    ("GPIOx",    GPIOB)
            ->withUnsignedIntParameters("GPIO_Pin", GPIO_PIN_6)
            ->withUnsignedIntParameters("PinState", GPIO_PIN_SET);
    io_setHigh (IO_LED_GREEN);
}


TEST (driver_io, set_pin_low){
    mock_c()->expectOneCall("HAL_GPIO_WritePin")
            ->withPointerParameters    ("GPIOx",    GPIOB)
            ->withUnsignedIntParameters("GPIO_Pin", GPIO_PIN_6)
            ->withUnsignedIntParameters("PinState", GPIO_PIN_RESET);
    io_setLow (IO_LED_GREEN);
}