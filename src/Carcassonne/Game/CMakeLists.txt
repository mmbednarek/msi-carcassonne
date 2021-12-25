project(msi_carcassonne_game)
add_library(msi_carcassonne_game Game.cpp Board.cpp Move.cpp Game_GPU.cpp Board_GPU.cpp Move_GPU.cpp)
target_link_libraries(msi_carcassonne_game libmb fmt spdlog)
