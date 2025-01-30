cmake_minimum_required(VERSION 3.21)
project(test VERSION 0.1 LANGUAGES CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIRS ON)

# suppress warnings from apple linker
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

####################################################################################################

set(CMAKE_OSX_DEPLOYMENT_TARGET 13)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

# set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Developer ID Application")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual")
####################################################################################################

find_package(Qt6 6.7 NO_MODULE REQUIRED COMPONENTS Quick)  

qt_standard_project_setup()
qt_add_executable(
    test
    main.cpp
    resources.qrc
)

target_link_libraries(
    test PRIVATE Qt6::Quick
)

# This generates app bundle
set_target_properties(test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.iamserj.test
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)
