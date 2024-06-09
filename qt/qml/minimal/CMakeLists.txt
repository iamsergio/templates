cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

option (USE_QT6 "Use Qt6" OFF)

if (USE_QT6)
    find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Quick)  
    set(QTMAJOR 6)
else()
    find_package(Qt5 5.15 NO_MODULE REQUIRED COMPONENTS Quick)
    set(QTMAJOR 5)
endif()


add_executable(
    test
    main.cpp
    resources.qrc
)

target_link_libraries(
    test Qt${QTMAJOR}::Quick
)
