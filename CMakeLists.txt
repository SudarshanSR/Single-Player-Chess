cmake_minimum_required(VERSION 3.28)
project(chess)

set(CMAKE_CXX_STANDARD 26)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_subdirectory(src/)

add_executable(chess src/main.cpp)
target_link_libraries(chess PRIVATE ui Qt::Widgets)