cmake_minimum_required(VERSION 3.21)
project(qnetworkproxy_test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.6 NO_MODULE REQUIRED COMPONENTS Widgets Network)

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(
    qnetworkproxy_test
    main.cpp
)

target_link_libraries(
    qnetworkproxy_test Qt6::Core Qt6::Network
)
