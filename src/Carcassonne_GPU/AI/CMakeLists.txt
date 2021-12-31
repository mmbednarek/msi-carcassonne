CUDA_ADD_LIBRARY(carcassonne_gpu_ai RandomPlayer.cu MCTSPlayer.cu Tree.cu Node.cu HeuristicPlayer.cu MCTS.cu DeepRL.cu DeepRLPlayer.cu STATIC)
target_link_libraries(carcassonne_gpu_ai carcassonne_gpu_rl libmb fmt caffe protobuf)
