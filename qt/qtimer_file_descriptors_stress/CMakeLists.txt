cmake_minimum_required(VERSION 3.21)
project(qtimer_stress)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.6 NO_MODULE REQUIRED COMPONENTS Widgets)

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(
    qtimer_stress
    main.cpp
)

target_link_libraries(
    qtimer_stress Qt6::Core
)
