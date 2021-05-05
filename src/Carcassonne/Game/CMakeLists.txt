project(msi_carcassonne_game)
add_library(msi_carcassonne_game Game.cpp Board.cpp Move.cpp)
target_link_libraries(msi_carcassonne_game libmb)
