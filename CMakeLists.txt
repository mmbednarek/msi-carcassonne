cmake_minimum_required(VERSION 3.9)
project(msi_carcassonne)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O3)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fconcepts)
endif()

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(include)

set(BUILD_GMOCK OFF)

add_subdirectory(deps/gtest)
add_subdirectory(deps/mb)
add_subdirectory(deps/fmt)

add_subdirectory(src/Graphics)
add_subdirectory(src/Input)
add_subdirectory(src/Carcassonne/Game)
add_subdirectory(src/Carcassonne/AI)
add_subdirectory(src/Carcassonne/Frontend)
add_subdirectory(src/bin/carcassonne)
add_subdirectory(src/bin/evolution)
add_subdirectory(src/bin/tournament)
add_subdirectory(src/Tournament)
add_subdirectory(src/Evolution)
add_subdirectory(src/Test)
