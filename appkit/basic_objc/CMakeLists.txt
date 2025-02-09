# SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.22)

project(webkit_qtwidgets)

enable_language(CXX)

find_package(Qt6 6.7 NO_MODULE REQUIRED COMPONENTS Widgets)
find_library(COCOA_LIBRARY Cocoa)

set(SRCS main.mm
)

add_executable(webkit_qtwidgets ${SRCS})
target_link_libraries(webkit_qtwidgets
    Qt6::Widgets
    ${COCOA_LIBRARY}
)

if(ENABLE_ASAN)
    target_compile_options(webkit_qtwidgets PRIVATE -fsanitize=address)
    target_link_options(webkit_qtwidgets PRIVATE -fsanitize=address)
endif()
