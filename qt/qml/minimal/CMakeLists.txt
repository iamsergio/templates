cmake_minimum_required(VERSION 3.21)
project(test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

option (USE_QT6 "Use Qt6" ON)

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

SET(USE_CUSTOM_LIB ON)

if (USE_CUSTOM_LIB)

    SET(MY_MODULE_TARGET_NAME mymodule)

    target_link_libraries(
        test PUBLIC Qt${QTMAJOR}::Quick mymodule
    )

    add_subdirectory(MyTest)
else()

    SET(MY_MODULE_TARGET_NAME test)
    target_link_libraries(test PUBLIC Qt${QTMAJOR}::Quick)

    QT_POLICY(SET QTP0001 NEW)
    QT_POLICY(SET QTP0004 NEW)
       
    qt_add_qml_module(test
        URI MyTest
        QML_FILES
            MyTest/MyRect.qml
            MyTest/main.qml
        SOURCES
            Obj.cpp
        NO_GENERATE_PLUGIN_SOURCE
        NO_PLUGIN
    )
    

endif()