cmake_minimum_required(VERSION 3.21)
project(fastbins_test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.6 NO_MODULE REQUIRED COMPONENTS Widgets)

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(
    fastbins_test
    main.cpp
)

target_link_libraries(
    fastbins_test Qt6::Widgets
)
