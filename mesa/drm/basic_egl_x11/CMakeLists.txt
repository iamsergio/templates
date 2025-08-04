cmake_minimum_required(VERSION 3.21)
project(drm_egl_example CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(drm_egl_example main.cpp)

target_include_directories(drm_egl_example PRIVATE /usr/include/libdrm)
target_link_libraries(drm_egl_example PRIVATE gbm drm EGL GL X11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -O0 -g -ggdb3")
