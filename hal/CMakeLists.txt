cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "../../external/robot-controller/robot_middleware/platforms/pico-armv6m/pico-sdk")
set(PICO_BOARD "pico_w")

# Must be before project()
include(pico_sdk_import.cmake)

project(robot_remote_controller C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------
# Tell CMake where to find the Pico builds of dependencies
# ------------------------------------------------------------

set(RM_PICO_PLATFORM_LIB_DIR "../external/robot-controller/robot_middleware/platforms/pico-armv6m")
set(MTP32_PICO_PLATFORM_LIB_DIR "../external/MTP32/lib/platforms/pico-armv6m")
set(RF24_DIR "../external/RF24")

# This is needed in case there is already an MTP32 package installed on the host computer.
# It helps CMake know which MTP32 package to look for. That being: the pico-armv6m one.
set(mtp32_DIR "../external/MTP32/lib/platforms/pico-armv6m/lib/cmake/mtp32")

list(APPEND CMAKE_PREFIX_PATH ${RM_PICO_PLATFORM_LIB_DIR})
List(APPEND CMAKE_PREFIX_PATH ${MTP32_PICO_PLATFORM_LIB_DIR})

list(APPEND CMAKE_FIND_ROOT_PATH ${RM_PICO_PLATFORM_LIB_DIR})
list(APPEND CMAKE_FIND_ROOT_PATH ${MTP32_PICO_PLATFORM_LIB_DIR})

pico_sdk_init()

find_package(robot_middleware REQUIRED)
find_package(mtp32 REQUIRED)

# Find and create RF24 library cmake target
# ------------------------------------------------------------
add_library(RF24 STATIC
    ${RF24_DIR}/RF24.cpp
    ${RF24_DIR}/utility/rp2/gpio.cpp
    ${RF24_DIR}/utility/rp2/spi.cpp
)

target_include_directories(RF24 PUBLIC
    ${RF24_DIR}
    ${RF24_DIR}/utility
    ${RF24_DIR}/utility/rp2
)

# RF24 requires Pico SDK SPI + GPIO + stdlib
target_link_libraries(RF24 PUBLIC
    pico_stdlib
    hardware_spi
    hardware_gpio
)
# ------------------------------------------------------------

add_subdirectory(link_manager)
add_subdirectory(joystick_driver)
add_subdirectory(component_manager)
add_subdirectory(src)