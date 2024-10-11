cmake_minimum_required(VERSION 3.22)

project(swift_webkit)

enable_language(Swift)

set(SRCS main.swift)

set(MY_LIB_SRCS mywebkitlib.swift)

add_library(mywebkitlib SHARED ${MY_LIB_SRCS})

add_executable(swift_webkit ${SRCS})
target_link_libraries(swift_webkit mywebkitlib)
