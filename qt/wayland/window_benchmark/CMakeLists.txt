
cmake_minimum_required(VERSION 3.21)
project(minimal-qml-compositor LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(compositor-benchmark
    main.cpp
)

qt_add_executable(client
    main_client.cpp
)

target_link_libraries(compositor-benchmark PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
)

target_link_libraries(client PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
)

qt6_add_resources(compositor-benchmark "compositor"
    PREFIX "/"
    FILES main.qml
)

qt6_add_resources(client "client"
    PREFIX "/"
    FILES main_client.qml
)
