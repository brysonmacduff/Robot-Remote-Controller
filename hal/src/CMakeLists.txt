set(COMPONENT ${PROJECT_NAME})

file(GLOB CPP_FILES "*.h" "*.cpp")

# ------------------------------------------------------------
# Firmware target
# ------------------------------------------------------------

add_executable(${COMPONENT} ${CPP_FILES})

target_link_libraries(${COMPONENT} PUBLIC pico_stdlib robot_middleware::robot_middleware)

pico_add_extra_outputs(${COMPONENT})
