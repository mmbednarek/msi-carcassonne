cmake_minimum_required(VERSION 3.9)
project(msi_carcassonne)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)

add_subdirectory(deps/mb)
add_subdirectory(deps/fmt)

add_subdirectory(src/Graphics)
add_subdirectory(src/Input)
add_subdirectory(src/Carcassonne/Game)
add_subdirectory(src/Carcassonne/Frontend)
add_subdirectory(src/bin/carcassonne)
add_subdirectory(src/Test)
