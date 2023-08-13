cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(led_dimmer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(led_dimmer
	led_dimmer.c
)

pico_enable_stdio_usb(led_dimmer 1)
pico_enable_stdio_uart(led_dimmer 1)

pico_add_extra_outputs(led_dimmer)

target_link_libraries(led_dimmer pico_stdlib hardware_pwm)
