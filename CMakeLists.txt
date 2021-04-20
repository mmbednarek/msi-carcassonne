cmake_minimum_required(VERSION 3.17)
project(msi_carcassonne)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(fmt REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)

add_subdirectory(src/Graphics)
add_subdirectory(src/Game)
add_subdirectory(src/bin/carcassonne)
