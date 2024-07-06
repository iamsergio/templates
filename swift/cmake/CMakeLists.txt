project(swift_cmake_example)

cmake_minimum_required(VERSION 3.22)

enable_language(Swift)

set(SRCS main.swift)

add_executable(swift_cmake_example ${SRCS})
