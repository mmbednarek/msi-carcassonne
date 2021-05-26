project(msi_carcassonne_ai)
add_library(msi_carcassonne_ai RandomPlayer.cpp MCTSPlayer.cpp)
target_link_libraries(msi_carcassonne_ai libmb fmt)
