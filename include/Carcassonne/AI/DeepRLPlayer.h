#ifndef MSI_CARCASSONNE_DEEPRL_PLAYER_H
#define MSI_CARCASSONNE_DEEPRL_PLAYER_H
#pragma once
#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/IGame.h>
#include <Carcassonne/RL/Network.h>
#include <random>
#include <thread>

namespace carcassonne::ai {

class DeepRLPlayer {
   std::array<FullMove, 4> m_last_moves{};
   mb::size m_player_count;
   Player m_player;
   std::shared_ptr<rl::DataWrapper<rl::MoveReadyness>> m_move_readyness;
   std::shared_ptr<std::condition_variable> m_ready_to_move;
   std::shared_ptr<std::condition_variable> m_move_found;
   std::mt19937 m_generator;
   std::unique_ptr<rl::Context> m_ctx_ptr = nullptr;
   unsigned m_trees_count = 1;
   std::unique_ptr<rl::client_threads> m_clients_pool = nullptr;

 public:
   explicit DeepRLPlayer(
      IGame &game,
      Player player,
      std::mt19937 &generator,
      std::unique_ptr<carcassonne::ai::rl::thread_pool>& workers_pool,
      unsigned trees_count );
   DeepRLPlayer() = delete;
   DeepRLPlayer(const DeepRLPlayer&) = delete;
   DeepRLPlayer(DeepRLPlayer&&) = default;
   ~DeepRLPlayer() {
      spdlog::info("~DeepRLPlayer: terminating threads");
      m_ctx_ptr->move_readyness->terminate = true;
      m_ctx_ptr->ready_to_move->notify_one();
      await_for_termination(m_ctx_ptr->move_readyness, m_ctx_ptr->move_found);
      m_clients_pool->join_clients();
      spdlog::info("~DeepRLPlayer: terminated threads");
   }
   void make_move(IGame &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
