set(TEST_TARGET ${COMPONENT}.test)

file(GLOB CPP_FILES "*.h" "*.cpp")

add_executable(${TEST_TARGET} ${CPP_FILES})

target_link_libraries(${TEST_TARGET} PUBLIC joystick_driver pico_cyw43_arch_none)

pico_add_extra_outputs(${TEST_TARGET})
pico_enable_stdio_usb(${TEST_TARGET} 1)
pico_enable_stdio_uart(${TEST_TARGET} 0)
