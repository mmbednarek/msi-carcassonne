#ifndef MSI_CARCASSONNE_DEEPRL_H
#define MSI_CARCASSONNE_DEEPRL_H
#include "AI.h"
#include <Carcassonne/Player.h>
#include <Carcassonne/RL/Network.h>
#include <mb/result.h>
#include <Carcassonne/RL/Concurrency.h>

namespace carcassonne {
struct FullMove;
}

namespace carcassonne::ai {

class Tree;

namespace rl {

struct Context {
   thread_pool& workers_pool;
   std::mutex mutex;
   std::unique_lock<std::mutex> lck{mutex, std::defer_lock};
};


mb::result<FullMove> find_non_idiotic(Tree &tree, Player player);
void simulate(Context &ctx, NodeId node_id);
void expand(Context &ctx, NodeId node_id);
void backpropagate(Context &ctx, NodeId node_id, Player winner);
void run_selection(Context &ctx);
void run_mcts(Context &ctx, mb::i64 time_limit, mb::i64 runs_limit);
FullMove choose_move(Context &ctx, int move_index, Player player);

}

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_H
