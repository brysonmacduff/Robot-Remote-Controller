set(COMPONENT "component_manager")

file(GLOB CPP_FILES "*.h" "*.cpp")

add_library(${COMPONENT} ${CPP_FILES})
target_link_libraries(${COMPONENT} PUBLIC joystick_driver link_manager app pico_cyw43_arch_none)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
