cmake_minimum_required(VERSION 3.9)
project(msi_carcassonne)

set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(include)

add_subdirectory(deps/googletest)
add_subdirectory(deps/mb)
add_subdirectory(deps/fmt)

add_subdirectory(src/Graphics)
add_subdirectory(src/Input)
add_subdirectory(src/Carcassonne/Game)
add_subdirectory(src/Carcassonne/Frontend)
add_subdirectory(src/bin/carcassonne)
add_subdirectory(src/Test)
