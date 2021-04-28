cmake_minimum_required(VERSION 3.17)
project(msi_carcassonne)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include(FetchContent)

FetchContent_Declare(
        libmb
        GIT_REPOSITORY https://github.com/mmbednarek/mb
        GIT_TAG libmb-0.0.16
)
FetchContent_MakeAvailable(libmb)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 7.1.3
)
FetchContent_MakeAvailable(fmt)

include_directories(include)

add_subdirectory(src/Graphics)
add_subdirectory(src/Input)
add_subdirectory(src/Carcassonne/Game)
add_subdirectory(src/Carcassonne/Frontend)
add_subdirectory(src/bin/carcassonne)
add_subdirectory(src/Test)
