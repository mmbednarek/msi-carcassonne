#ifndef MSI_CARCASSONNE_DEEPRL_PLAYER_H
#define MSI_CARCASSONNE_DEEPRL_PLAYER_H
#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/IGame.h>
#include <Carcassonne/RL/Network.h>
#include <random>

namespace carcassonne::ai {

class DeepRLPlayer {
   Player m_player;
   std::array<FullMove, 4> m_last_moves{};
   carcassonne::rl::Network &m_network;
   Tree m_tree;
   mb::size m_player_count;

   void prepare_tree(const IGame &game);

 public:
   explicit DeepRLPlayer(IGame &game, Player player, carcassonne::rl::Network &net);
   void train_network(const IGame &game);
   void make_move(IGame &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
