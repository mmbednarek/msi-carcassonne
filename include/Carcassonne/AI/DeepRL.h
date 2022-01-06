#ifndef MSI_CARCASSONNE_DEEPRL_H
#define MSI_CARCASSONNE_DEEPRL_H
#pragma once
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
   // Tree &tree;
   IGame &game;
   Player player;
   std::array<FullMove, 4> &last_moves;
   std::map<std::thread::id, std::unique_ptr<Tree>> trees;
   thread_pool& workers_pool;
   std::thread::id leading_tread_id;
   bool leading_tread_assigned = false;
   std::mutex mutex;
   std::unique_lock<std::mutex> lck{mutex, std::defer_lock};
   FullMove best_move;
   bool move_ready = false;

 public:
   Context(IGame &_game, Player &_player, std::array<FullMove, 4>& _last_moves, thread_pool& _workers_pool)
       : game(_game), player(_player), last_moves(_last_moves), workers_pool(_workers_pool) {}
};

void launch_simulations(std::unique_ptr<rl::Context> &ctx_ptr, NodePtr node_id);
void expand(std::unique_ptr<rl::Context> &ctx_ptr, NodePtr node_id);
void backpropagate(
        NodePtr node_id,
        Player winner,
        std::unique_ptr<Tree> &tree);
void run_selection(std::unique_ptr<rl::Context> &ctx_ptr);
void run_mcts(std::unique_ptr<rl::Context> &ctx_ptr, mb::i64 time_limit, mb::i64 runs_limit);
FullMove choose_move(std::unique_ptr<rl::Context> &ctx_ptr, int move_index);

}

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_H
