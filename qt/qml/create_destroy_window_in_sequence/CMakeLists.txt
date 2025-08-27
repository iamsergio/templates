cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

option (USE_QT6 "Use Qt6" ON)
option (USE_JEMALLOC "Use Jemalloc" OFF)

if (USE_QT6)
    find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Quick)
    set(QTMAJOR 6)
else()
    find_package(Qt5 5.15 NO_MODULE REQUIRED COMPONENTS Quick)
    set(QTMAJOR 5)
endif()


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(
    test
    main.cpp
)


target_link_libraries(test PUBLIC Qt${QTMAJOR}::Quick)

QT_POLICY(SET QTP0001 NEW)

if(QT_VERSION VERSION_GREATER_EQUAL "6.5.0")
    QT_POLICY(SET QTP0004 NEW)
endif()

include_directories(${CMAKE_SOURCE_DIR}/MyTest)

qt_add_qml_module(test
    URI MyTest
    QML_FILES
        MyTest/MyRect.qml
        MyTest/main.qml
    SOURCES
        MyTest/Obj.cpp
    NO_PLUGIN
)
