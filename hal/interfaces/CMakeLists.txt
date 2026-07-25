set(COMPONENT "interfaces")

file(GLOB CPP_FILES "*.h" "*.cpp")

add_library(${COMPONENT} ${CPP_FILES})
target_link_libraries(${COMPONENT} PUBLIC robot_middleware::robot_middleware)
target_include_directories(${COMPONENT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
