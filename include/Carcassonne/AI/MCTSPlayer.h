#ifndef MSI_CARCASSONNE_MCTS_PLAYER_H
#define MSI_CARCASSONNE_MCTS_PLAYER_H
#include "Node.h"
#include "Tree.h"
#include <Carcassonne/IGame.h>
#include <random>

namespace carcassonne::ai {

class MCTSPlayer {
   Player m_player;
 public:
   explicit MCTSPlayer(IGame &game, Player player);

   void make_move(IGame &game) noexcept;
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_MCTS_PLAYER_H
