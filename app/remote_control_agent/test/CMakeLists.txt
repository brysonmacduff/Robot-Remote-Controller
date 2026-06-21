set(TEST_TARGET ${COMPONENT}.test)

file(GLOB CPP_FILES "*.h" "*.cpp")

add_executable(${TEST_TARGET} ${CPP_FILES})
target_link_libraries(${TEST_TARGET} PRIVATE ${COMPONENT} GTest::gtest_main)