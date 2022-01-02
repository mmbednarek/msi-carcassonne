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
   mb::size m_gpus, m_cpus;

   void prepare_tree(const IGame &game);

 public:
   explicit DeepRLPlayer(
      IGame &game,
      Player player,
      carcassonne::rl::Network &net,
      mb::size gpus,
      mb::size cpus );
   void train_network(const IGame &game); // to be implemented
   void make_move(IGame &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
