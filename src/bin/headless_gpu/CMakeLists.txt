add_executable(carcassonne_gpu_headless main.cpp)
target_link_libraries(carcassonne_gpu_headless cudatest fmt Boost::program_options spdlog ${CUDA_LIBRARIES})
