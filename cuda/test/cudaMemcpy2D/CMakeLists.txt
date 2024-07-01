project(hello_cuda LANGUAGES CXX CUDA)

find_package(CUDAToolkit REQUIRED)

add_executable(hello_cu main.cpp)
target_link_libraries(hello_cu CUDA::cudart)
