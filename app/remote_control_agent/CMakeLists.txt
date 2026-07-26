set(COMPONENT "remote_control_agent")

file(GLOB CPP_FILES "*.h" "*.cpp")

add_library(${COMPONENT} ${CPP_FILES})

# Public headers for consumers (HAL)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${COMPONENT} PUBLIC robot_middleware::robot_middleware hal_interfaces)

# Unit tests
add_subdirectory(test)
