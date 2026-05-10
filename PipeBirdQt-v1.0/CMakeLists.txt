cmake_minimum_required(VERSION 3.16)

project(PipeBirdQt VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(PipeBirdQt
    src/main.cpp
    src/gamewidget.cpp
    src/bird.cpp
    src/pipe.cpp
)

target_include_directories(PipeBirdQt PRIVATE include)

target_link_libraries(PipeBirdQt PRIVATE Qt6::Widgets)
