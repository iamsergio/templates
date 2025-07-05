cmake_minimum_required(VERSION 3.10)

project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost libraries (header-only for pool)
find_package(Boost REQUIRED)

set(SRCS
    main.cpp
)

add_executable(bench ${SRCS})

# Link Boost headers
target_include_directories(bench PRIVATE ${Boost_INCLUDE_DIRS})

# target_compile_definitions(example MYDEFINE)
# target_include_directories(example PRIVATE ..)
# target_link_libraries(example <libraries>)
