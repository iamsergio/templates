project(hello_npp LANGUAGES CXX CUDA)

find_package(CUDAToolkit REQUIRED)

add_executable(hello_npp main.cpp)
target_link_libraries(hello_npp
           CUDA::cudart
           CUDA::nppc
           CUDA::nppif
           CUDA::nppist
           CUDA::nppisu
           CUDA::nppim)
