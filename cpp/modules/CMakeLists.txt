cmake_minimum_required(VERSION 3.28)
project(modules_fib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(modules_fib)

target_sources(modules_fib
  PRIVATE
    main.cpp
  PRIVATE
    FILE_SET CXX_MODULES FILES
    my_module_example.cppm
)
