#ifndef CARCASSONNE_GPU_DEEPRL_PLAYER_H
#define CARCASSONNE_GPU_DEEPRL_PLAYER_H

#include <Carcassonne_GPU/AI/DeepRL.h>
#include <Carcassonne_GPU/AI/Tree.h>
#include <Carcassonne_GPU/Game/Game.h>
#include <Carcassonne_GPU/RL/Network.h>
#include <random>

namespace carcassonne::ai {

class DeepRLPlayer {
   Player m_player;
   std::array<game::FullMove, 4> m_last_moves{};
   carcassonne::rl::Network &m_network;
   Tree m_tree;
   mb::size m_player_count;

   void prepare_tree(const game::Game &game);

 public:
   explicit DeepRLPlayer(game::Game &game, Player player, carcassonne::rl::Network &net);
   void train_network(const game::Game &game);
   void make_move(game::Game &game) noexcept;
};

}// namespace carcassonne::ai

#endif//CARCASSONNE_GPU_DEEPRL_PLAYER_H
