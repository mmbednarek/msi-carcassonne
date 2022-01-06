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
   Player m_player;
   std::array<FullMove, 4> m_last_moves{};
   mb::size m_player_count;
   std::unique_ptr<rl::client_threads> m_clients_pool = nullptr;
   std::mt19937 m_generator;

 public:
   explicit DeepRLPlayer(
      IGame &game,
      Player player,
      std::mt19937 &generator);
   void make_move(IGame &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
