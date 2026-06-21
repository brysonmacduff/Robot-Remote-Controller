cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH "../../external/robot-controller/robot_middleware/platforms/pico-armv6m/pico-sdk")
set(PICO_BOARD "pico_w")

# Must be before project()
include(pico_sdk_import.cmake)

project(robot_remote_controller C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------
# Tell CMake where to find the Pico build of robot_middleware
# ------------------------------------------------------------

set(RM_PICO_PLATFORM_LIB_DIR "../external/robot-controller/robot_middleware/platforms/pico-armv6m")

list(APPEND CMAKE_PREFIX_PATH ${RM_PICO_PLATFORM_LIB_DIR})

list(APPEND CMAKE_FIND_ROOT_PATH ${RM_PICO_PLATFORM_LIB_DIR})

pico_sdk_init()

find_package(robot_middleware REQUIRED)

add_subdirectory(link_manager)
add_subdirectory(joystick_driver)
add_subdirectory(component_manager)
add_subdirectory(src)