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

   void prepare_tree(const std::unique_ptr<IGame> &game);

 public:
   explicit DeepRLPlayer(std::unique_ptr<IGame> &game, Player player, carcassonne::rl::Network &net);
   void train_network(const std::unique_ptr<IGame> &game);
   void make_move(std::unique_ptr<IGame> &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
