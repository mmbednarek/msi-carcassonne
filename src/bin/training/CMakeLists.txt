add_executable(carcassonne_training main.cpp)
target_link_libraries(carcassonne_training msi_carcassonne_game msi_carcassonne_ai carcassonne_encoder fmt Boost::program_options spdlog pthread)