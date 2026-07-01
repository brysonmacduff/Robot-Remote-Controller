set(COMPONENT "link_manager")

file(GLOB CPP_FILES "*.h" "*.cpp")

add_library(${COMPONENT} ${CPP_FILES})

target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${COMPONENT} PUBLIC pico_stdlib robot_middleware::robot_middleware mtp32::mtp32 RF24)
set_target_properties(${COMPONENT} PROPERTIES LINKER_LANGUAGE CXX)