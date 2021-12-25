project(msi_carcassonne_ai)
add_library(msi_carcassonne_ai RandomPlayer.cpp MCTSPlayer.cpp Tree.cpp Node.cpp HeuristicPlayer.cpp MCTS.cpp DeepRL.cpp DeepRLPlayer.cpp)
target_link_libraries(msi_carcassonne_ai libmb fmt caffe protobuf carcassonne_rl spdlog)
