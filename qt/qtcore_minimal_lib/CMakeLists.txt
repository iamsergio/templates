cmake_minimum_required(VERSION 3.21)
project(myqtlib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

find_package(Qt6 6.6 NO_MODULE REQUIRED COMPONENTS Widgets)

if (ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

include(GenerateExportHeader)
add_library(myqtlib SHARED
    lib.cpp
)

generate_export_header(
    myqtlib EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/myqtlib_export.h" EXPORT_MACRO_NAME MYLIB_EXPORT
)

target_link_libraries(
    myqtlib Qt6::Core
)

target_include_directories(myqtlib PUBLIC ${CMAKE_BINARY_DIR})
