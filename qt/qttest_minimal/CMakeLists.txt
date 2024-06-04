cmake_minimum_required(VERSION 3.21)
project(qttest_minimal)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Test)

add_executable(
    qttest_minimal
    test.cpp
)

target_link_libraries(
    qttest_minimal Qt6::Test
)
