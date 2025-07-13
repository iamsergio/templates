
cmake_minimum_required(VERSION 3.21)
project(minimal-qml-compositor LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml)

qt_add_executable(compositor
    main.cpp
)

target_link_libraries(compositor PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
)

qt6_add_resources(compositor "compositor"
    PREFIX "/"
    FILES main.qml
)
