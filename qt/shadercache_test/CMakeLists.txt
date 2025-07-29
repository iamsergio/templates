cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

option (USE_QT6 "Use Qt6" ON)

if (USE_QT6)
    find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Widgets Quick OpenGL OpenGLWidgets QuickPrivate)
    set(QTMAJOR 6)
else()
    find_package(Qt5 5.15 NO_MODULE REQUIRED COMPONENTS Widgets Quick OpenGL OpenGLWidgets)
    set(QTMAJOR 5)
endif()


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(
    test_qopenglprogram
    main_qopenglprogram.cpp
)

qt_add_executable(
    test_qquickwindow
    main_qquickwindow.cpp
)

target_link_libraries(test_qopenglprogram PUBLIC Qt${QTMAJOR}::Quick Qt${QTMAJOR}::Widgets Qt${QTMAJOR}::OpenGLWidgets)
target_link_libraries(test_qquickwindow PUBLIC Qt${QTMAJOR}::QuickPrivate Qt${QTMAJOR}::Quick Qt${QTMAJOR}::OpenGLWidgets)

QT_POLICY(SET QTP0001 NEW)
QT_POLICY(SET QTP0004 NEW)
    
qt_add_qml_module(test_qquickwindow
    URI MyTest
    QML_FILES
        MyTest/MyRect.qml
        MyTest/main.qml
    # SOURCES
    #     MyTest/Obj.cpp
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN
)
