project(swift_cmake_example)

cmake_minimum_required(VERSION 3.22)

enable_language(Swift)

set(SRCS main.swift)

set(SOME_LIBRARY_SRCS mylibrary.swift)
set(SOME_STATIC_LIBRARY_SRCS mystaticlibrary.swift)

add_library(mylibrary SHARED ${SOME_LIBRARY_SRCS})
add_library(mystaticlibrary STATIC ${SOME_STATIC_LIBRARY_SRCS})

add_executable(swift_cmake_example ${SRCS})
target_link_libraries(swift_cmake_example mystaticlibrary mylibrary)

add_executable(cpp_to_swift main.cpp)
target_link_libraries(cpp_to_swift mystaticlibrary mylibrary)