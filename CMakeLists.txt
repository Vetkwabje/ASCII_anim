cmake_minimum_required(VERSION 3.16)

project(ASCII_anim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

add_executable(ASCII_anim
    src/main.cpp
    src/animation.cpp
    src/renderer.cpp
    src/terminal.cpp
)

target_include_directories(ASCII_anim PRIVATE
    include
    ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(ASCII_anim PRIVATE
    ${CURSES_LIBRARIES}
)