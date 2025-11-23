cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)


find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Quick QuickControls2)  
set(QTMAJOR 6)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(
    test
    main.cpp
)


SET(MY_MODULE_TARGET_NAME mymodule)

target_link_libraries(
    test PUBLIC Qt${QTMAJOR}::Quick Qt${QTMAJOR}::QuickControls2 mymodule
)

add_subdirectory(MyTest)
