cmake_minimum_required(VERSION 3.10)

project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
    main.cpp
)

add_executable(example ${SRCS})

# target_compile_definitions(example MYDEFINE)
# target_include_directories(example PRIVATE ..)
# target_link_libraries(example <libraries>)
