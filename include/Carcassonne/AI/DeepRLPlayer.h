#ifndef MSI_CARCASSONNE_DEEPRL_PLAYER_H
#define MSI_CARCASSONNE_DEEPRL_PLAYER_H
#include "Carcassonne/Game/Game.h"
#pragma once
#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/IGame.h>
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/Training/TrainingSet.h>
#include <random>
#include <thread>

namespace carcassonne::ai {

class DeepRLPlayer {
   std::pair<std::unique_ptr<IGame>, training::OneGame> &m_game_with_training_data;
   std::array<FullMove, 4> m_last_moves{};
   mb::size m_player_count;
   Player m_player;
   std::shared_ptr<rl::DataWrapper<rl::MoveReadyness>> m_move_readyness;
   std::shared_ptr<std::condition_variable> m_ready_to_move;
   std::shared_ptr<std::condition_variable> m_move_found;
   std::mt19937 m_generator;
   training::MoveNetworkRecord m_record{};
   std::unique_ptr<rl::Context> m_ctx_ptr = nullptr;
   unsigned m_trees_count = 1;
   std::unique_ptr<rl::client_threads> m_clients_pool = nullptr;

 public:
   explicit DeepRLPlayer(
      std::pair<std::unique_ptr<IGame>, training::OneGame> &game_with_training_data,
      Player player,
      std::mt19937 &generator,
      std::unique_ptr<carcassonne::ai::rl::thread_pool>& workers_pool,
      unsigned trees_count
   );

   DeepRLPlayer(const DeepRLPlayer&) = delete;
   DeepRLPlayer &operator=(const DeepRLPlayer&) = delete;
   ~DeepRLPlayer();
   void add_record(std::pair<std::unique_ptr<IGame>, training::OneGame> &game_with_training_data, Node* node_with_best_move);
   void make_move(IGame &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
