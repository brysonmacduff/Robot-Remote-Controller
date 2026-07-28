cmake_minimum_required(VERSION 3.13)

project(app C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------
# Find dependencies
# ------------------------------------------------------------

find_package(robot_middleware REQUIRED)
find_package(GTest REQUIRED)

# Find hal_interfaces as an installed package
add_library(hal_interfaces INTERFACE)
target_include_directories(hal_interfaces INTERFACE "../hal/interfaces")

add_subdirectory(remote_control_agent)

# Copy over all app source and header files to the hal directory for consumption 

set(APP_FILES_DESTINATION_DIR "../hal/app")
file(MAKE_DIRECTORY ${APP_FILES_DESTINATION_DIR})

# Gather all APP headers and sources
file(GLOB APP_CODE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/remote_control_agent/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/remote_control_agent/*.cpp"
)

# Create symlinks in hal/app
foreach(CODE_FILE IN LISTS APP_CODE_FILES)
    get_filename_component(name "${CODE_FILE}" NAME)
    execute_process(
        COMMAND ln -sf "${CODE_FILE}" "${APP_FILES_DESTINATION_DIR}/${name}"
    )
endforeach()