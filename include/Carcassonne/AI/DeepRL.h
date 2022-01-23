#ifndef MSI_CARCASSONNE_DEEPRL_H
#define MSI_CARCASSONNE_DEEPRL_H
#pragma once
#include "AI.h"
#include <Carcassonne/Player.h>
#include <Carcassonne/RL/Network.h>
#include <mb/result.h>
#include <Carcassonne/RL/Concurrency.h>
#include <Carcassonne/Training/TrainingSet.h>

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
   std::unique_ptr<carcassonne::ai::rl::thread_pool> &workers_pool;
   std::shared_ptr<rl::DataWrapper<rl::MoveReadyness>> &move_readyness;
   std::shared_ptr<std::condition_variable> &ready_to_move;
   std::shared_ptr<std::condition_variable> &move_found;
   Node* node_with_best_move;
   std::map<std::thread::id, std::unique_ptr<Tree>> trees;
   std::thread::id leading_tread_id;
   bool leading_tread_assigned = false;
   std::mutex mutex;
   std::unique_lock<std::mutex> lck{mutex, std::defer_lock};

 public:
   Context(
     IGame &_game,
     Player &_player,
     std::array<FullMove, 4> &_last_moves,
     std::unique_ptr<carcassonne::ai::rl::thread_pool> &_workers_pool,
     std::shared_ptr<rl::DataWrapper<rl::MoveReadyness>> &_move_readyness,
     std::shared_ptr<std::condition_variable>& _ready_to_move,
     std::shared_ptr<std::condition_variable>& _move_found
   )
    : game(_game)
    , player(_player)
    , last_moves(_last_moves)
    , workers_pool(_workers_pool)
    , move_readyness(_move_readyness)
    , ready_to_move(_ready_to_move)
    , move_found(_move_found) {}
};

void launch_simulations(std::unique_ptr<rl::Context> &ctx_ptr, NodePtr node_id);
std::tuple<std::span<float>, float> get_probabilities(
   std::unique_ptr<rl::Context> &ctx_ptr,
   NodePtr node,
   std::vector<float> &neuron_input );
void expand(std::unique_ptr<rl::Context> &ctx_ptr, NodePtr node_id);
void backpropagate(
        NodePtr node_id,
        Player winner,
        std::unique_ptr<Tree> &tree);
void run_selection(std::unique_ptr<rl::Context> &ctx_ptr);
void run_mcts(std::unique_ptr<rl::Context> &ctx_ptr, mb::i64 time_limit, mb::i64 runs_limit);
Node* choose_move(std::unique_ptr<rl::Context> &ctx_ptr);

}

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_H
