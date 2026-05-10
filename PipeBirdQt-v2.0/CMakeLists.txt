cmake_minimum_required(VERSION 3.16)

project(PipeBirdQtPro VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

add_executable(PipeBirdQtPro
    resources.qrc
    include/bird.h
    include/pipe.h
    include/difficulty.h
    include/menupage.h
    include/gamewidget.h
    include/mainwindow.h
    src/main.cpp
    src/mainwindow.cpp
    src/menupage.cpp
    src/gamewidget.cpp
    src/bird.cpp
    src/pipe.cpp
)

target_include_directories(PipeBirdQtPro PRIVATE include)
target_link_libraries(PipeBirdQtPro PRIVATE Qt6::Widgets Qt6::Multimedia)
