cmake_minimum_required(VERSION 3.21)
project(main)

enable_language(CXX)

set(CMAKE_CXX_STANDARD 23)

set(srcs
    main.cpp
)

set(libsrcs
    lib.cpp
)

add_executable(main ${srcs} )
add_library(foo ${libsrcs})

target_link_libraries(main foo)
