#ifndef MSI_CARCASSONNE_MCTS_PLAYER_H
#define MSI_CARCASSONNE_MCTS_PLAYER_H
#include "Node.h"
#include "Tree.h"
#include <Carcassonne/IGame.h>
#include <random>

namespace carcassonne::ai {

class MCTSPlayer {
   Player m_player;

   std::random_device m_random_device;
   std::mt19937_64 m_random_generator;
 public:
   explicit MCTSPlayer(IGame &game, Player player);

   void make_move(IGame &game) noexcept;
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_MCTS_PLAYER_H
