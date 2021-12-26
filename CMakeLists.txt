cmake_minimum_required(VERSION 3.9)
project(msi_carcassonne)

set(CMAKE_CXX_STANDARD 20)
option(HIP_PLATFORM_AMD "should be built for AMD" ON)
set(CMAKE_CXX_FLAGS "-std=c++20 -O3 -g")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fconcepts)
endif()

# find_package(fmt REQUIRED)
# find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(include)
include_directories(deps/mb/include)
include_directories(include ${SDL2_INCLUDE_DIRS})

if(${HIP_PLATFORM_AMD})
    include_directories(/opt/rocm/hiprand/include)
    include_directories(/opt/rocm/rocrand/include)
    include_directories(/opt/rocm/hipblas/include)
    include_directories(/opt/rocm/hip/include)
    add_definitions(-D__HIP_PLATFORM_AMD__ -D__HIP_PLATFORM_HCC__)
endif()

set(BUILD_GMOCK OFF)

add_subdirectory(deps/mb)
add_subdirectory(deps/fmt)
add_subdirectory(deps/gtest)
add_subdirectory(deps/spdlog)

add_subdirectory(src/Graphics)
add_subdirectory(src/Input)
add_subdirectory(src/Carcassonne/Game)
add_subdirectory(src/Carcassonne/Decoder)
add_subdirectory(src/Carcassonne/Encoder)
add_subdirectory(src/Carcassonne/RL)
add_subdirectory(src/Carcassonne/AI)
add_subdirectory(src/Carcassonne/Frontend)
add_subdirectory(src/bin/carcassonne)
# add_subdirectory(src/bin/evolution)
add_subdirectory(src/bin/headless)
# add_subdirectory(src/Evolution)
add_subdirectory(src/Test)

find_library(caffe libcaffe ${CMAKE_LIBRARY_PATH})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(GLog REQUIRED)

find_package(Protobuf REQUIRED)
