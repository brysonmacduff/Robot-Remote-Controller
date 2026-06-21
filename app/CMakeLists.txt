cmake_minimum_required(VERSION 3.13)

project(robot_remote_controller C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(robot_middleware REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(remote_control_agent)