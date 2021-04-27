project(msi_carcassonne_frontend)
add_library(msi_carcassonne_frontend GameView.cpp Resource.cpp BoardView.cpp Tile.cpp PlaceTileView.cpp)
target_link_libraries(msi_carcassonne_frontend libmb msi_graphics msi_input)
