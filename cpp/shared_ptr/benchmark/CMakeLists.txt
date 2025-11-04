cmake_minimum_required(VERSION 3.16)

project(benchmark VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

set(SOURCES
    main.cpp
    foo.cpp
)

add_executable(benchmark_by_ref ${SOURCES})
set_target_properties(benchmark_by_ref PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_executable(benchmark_by_value ${SOURCES})
target_compile_definitions(benchmark_by_value PRIVATE BENCHMARK_BY_VALUE)
set_target_properties(benchmark_by_value PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)