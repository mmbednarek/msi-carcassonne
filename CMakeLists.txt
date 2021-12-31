cmake_minimum_required(VERSION 3.9)
project(carcassonne)

set(CMAKE_CXX_FLAGS "-O0 -Wall -g")
set(CMAKE_CXX_STANDARD 20)
option(HIP_PLATFORM_AMD "should be built for AMD" OFF)
option(MEASURE_TIME "measure time too?" OFF)

if (NOT ${HIP_PLATFORM_AMD})
    set(CUDA_ARCH "-arch=sm_80" CACHE STRING "Value of the NVCC -arch option.")
    if(${CMAKE_VERSION} VERSION_EQUAL 3.7 OR ${CMAKE_VERSION} VERSION_GREATER 3.7)
        option(AUTO_DETECT_ARCH "Detect local GPU compute arch automatically" ON)
    endif()
    message(STATUS "CUDA_ARCH: \"${CUDA_ARCH}\"")
    SET(CUDA_NVCC_FLAGS "--expt-relaxed-constexpr")
    SET(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -std=c++17 )

    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --ptxas-options=-v")# -allow-unsupported-compiler")

    SET(CUDA_PROPAGATE_HOST_FLAGS ON)
    SET(CUDA_SEPARABLE_COMPILATION ON)
    SET(CUDA_VERBOSE_BUILD ON)

    find_package(CUDA REQUIRED)
endif()
#   -ftemplate-backtrace-limit=0;
#   --cudart; 
#   static; 
#   -O4 ; 
#   --expt-relaxed-constexpr; 
#   -fmad=true; 
#   --expt-extended-lambda;
#   --relocatable-device-code=false;
#   -Xptxas ;
#   -dlcm=ca 
#   )

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fconcepts)
endif()


set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(include)
include_directories(deps/mb/include)
include_directories(deps/ecuda/include)
include_directories(include ${SDL2_INCLUDE_DIRS})

if(${HIP_PLATFORM_AMD})
    include_directories(/opt/rocm/hiprand/include)
    include_directories(/opt/rocm/rocrand/include)
    include_directories(/opt/rocm/hipblas/include)
    include_directories(/opt/rocm/hip/include)
    add_definitions(-D__HIP_PLATFORM_AMD__ -D__HIP_PLATFORM_HCC__)
else()
    include_directories(/usr/local/cuda-11.5/targets/x86_64-linux/include)
endif()

if(${MEASURE_TIME})
    add_definitions(-DMEASURE_TIME)
endif()

set(BUILD_GMOCK OFF)

add_subdirectory(deps/mb)

add_subdirectory(src/Proto)
add_subdirectory(src/Graphics)
add_subdirectory(src/Input)
add_subdirectory(src/Carcassonne/Game)
add_subdirectory(src/Carcassonne/Decoder)
add_subdirectory(src/Carcassonne/Encoder)
add_subdirectory(src/Carcassonne/RL)
add_subdirectory(src/Carcassonne/AI)
add_subdirectory(src/Carcassonne/Frontend)
# add_subdirectory(src/Carcassonne_GPU/Game)
# add_subdirectory(src/Carcassonne_GPU/Decoder)
# add_subdirectory(src/Carcassonne_GPU/Encoder)
# add_subdirectory(src/Carcassonne_GPU/RL)
# add_subdirectory(src/Carcassonne_GPU/AI)
add_subdirectory(src/bin/carcassonne)
add_subdirectory(src/bin/evolution)

if (NOT ${HIP_PLATFORM_AMD})
add_subdirectory(src/cudatest)
endif()
# add_subdirectory(src/bin/headless_gpu)
add_subdirectory(src/bin/headless)
add_subdirectory(src/Evolution)
add_subdirectory(src/Test)

find_library(caffe libcaffe ${CMAKE_LIBRARY_PATH})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(GLog REQUIRED)

find_package(Protobuf REQUIRED)
