#ifndef CARCASSONNE_GPU_DEEPRL_H
#define CARCASSONNE_GPU_DEEPRL_H
#include "AI.h"
#include <Carcassonne_GPU/Player.h>
#include <Carcassonne_GPU/RL/Network.h>

namespace carcassonne {
struct FullMove;
}

namespace carcassonne::ai {

class Tree;

struct Context {
   Tree &tree;
   rl::Network &network;
};


void simulate(Context &ctx, NodeId node_id);
void expand(Context &ctx, NodeId node_id);
void backpropagate(Context &ctx, NodeId node_id, Player winner);
void run_selection(Context &ctx);
void run_mcts(Context &ctx, mb::i64 time_limit);
game::FullMove choose_move(Context &ctx, int move_index, Player player);

}// namespace carcassonne::ai

#endif//CARCASSONNE_GPU_DEEPRL_H
