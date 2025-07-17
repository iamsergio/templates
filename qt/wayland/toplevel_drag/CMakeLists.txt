cmake_minimum_required(VERSION 3.21)
project(qtwidgets_basic)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt5 5.15 NO_MODULE REQUIRED COMPONENTS Widgets)

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(
    qtwidgets_basic
    main.cpp
)

target_link_libraries(
    qtwidgets_basic Qt5::Widgets
)
