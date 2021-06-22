#ifndef MSI_CARCASSONNE_MCTS_PLAYER_H
#define MSI_CARCASSONNE_MCTS_PLAYER_H
#include "Node.h"
#include "Tree.h"
#include <Carcassonne/IGame.h>
#include <Carcassonne/AI/MCTS.h>
#include <random>

namespace carcassonne::ai {

class MCTSPlayer {
   Player m_player;
   std::array<FullMove, 4> m_last_moves{};
   Tree m_tree;
   mb::size m_player_count;
   SimulationType m_simulation_type{};
 public:
   explicit MCTSPlayer(IGame &game, Player player, SimulationType sim_type);

   void prepare_tree(const IGame &game);
   void make_move(IGame &game) noexcept;
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_MCTS_PLAYER_H
