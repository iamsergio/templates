cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

option (USE_QT6 "Use Qt6" ON)

if (USE_QT6)
    find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS Widgets Quick OpenGL OpenGLWidgets)
    if(Qt6Core_VERSION VERSION_GREATER_EQUAL "6.10.0")
        find_package(Qt6 6.5 NO_MODULE REQUIRED COMPONENTS QuickPrivate)
    endif()
    
    set(QTMAJOR 6)
else()
    find_package(Qt5 5.15 NO_MODULE REQUIRED COMPONENTS Widgets Quick OpenGL OpenGLWidgets)
    set(QTMAJOR 5)
endif()

option(BUILD_QQUICKWINDOW "Build the QQuickWindow example" ON)
if (USE_QT6 AND Qt6Core_VERSION VERSION_GREATER_EQUAL "6.6.0")
    option (SUPPORTS_RHI "Enable RHI support" ON)
else()
    option (SUPPORTS_RHI "Enable RHI support" OFF)
endif()

if (SUPPORTS_RHI)
    add_definitions(-DSUPPORTS_RHI)
endif()

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(
    test_qopenglprogram
    main_qopenglprogram.cpp
)

target_link_libraries(test_qopenglprogram PUBLIC Qt${QTMAJOR}::Quick Qt${QTMAJOR}::Widgets Qt${QTMAJOR}::OpenGLWidgets)


if (BUILD_QQUICKWINDOW)
    qt_add_executable(
        test_qquickwindow
        main_qquickwindow.cpp
    )
    
    target_link_libraries(test_qquickwindow PUBLIC Qt${QTMAJOR}::QuickPrivate Qt${QTMAJOR}::Quick Qt${QTMAJOR}::OpenGLWidgets)
    
        
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
endif()
