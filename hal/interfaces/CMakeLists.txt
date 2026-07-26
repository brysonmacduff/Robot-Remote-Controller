set(COMPONENT "interfaces")

file(GLOB HEADER_FILES "*.h")

add_library(interfaces INTERFACE ${HEADER_FILES})

target_include_directories(interfaces INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

