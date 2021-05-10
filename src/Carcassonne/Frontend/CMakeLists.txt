project(msi_carcassonne_frontend)
add_library(msi_carcassonne_frontend GameView.cpp Resource.cpp BoardView.cpp Object.cpp PlaceTileView.cpp PlaceFigureView.cpp FiguresView.cpp)
target_link_libraries(msi_carcassonne_frontend -lm libmb msi_graphics msi_input)
