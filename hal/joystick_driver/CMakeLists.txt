set(COMPONENT "joystick_driver")

file(GLOB CPP_FILES "*.h" "*.cpp")

add_library(${COMPONENT} ${CPP_FILES})
target_link_libraries(${COMPONENT} PUBLIC pico_stdlib)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)