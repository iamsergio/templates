cmake_minimum_required(VERSION 3.21)
project(qtcore_basic)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.6 NO_MODULE REQUIRED COMPONENTS Widgets)

add_executable(
    qtcore_basic
    main.cpp
)

target_link_libraries(
    qtcore_basic Qt6::Core
)
