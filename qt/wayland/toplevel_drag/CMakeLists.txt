cmake_minimum_required(VERSION 3.21)
project(qtwidgets_basic)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.2 NO_MODULE REQUIRED COMPONENTS Widgets GuiPrivate)

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize=undefined)
  add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

add_executable(
    topleveldrag
    main.cpp
)

target_link_libraries(
    topleveldrag Qt6::Widgets Qt6::GuiPrivate
)
