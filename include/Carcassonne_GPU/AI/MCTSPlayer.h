#ifndef CARCASSONNE_GPU_MCTS_PLAYER_H
#define CARCASSONNE_GPU_MCTS_PLAYER_H

#include "Node.h"
#include "Tree.h"
#include <Carcassonne_GPU/Game/Game.h>
#include <Carcassonne_GPU/AI/MCTS.h>
#include <random>

namespace carcassonne::ai {

class MCTSPlayer {
   Player m_player;
   std::array<game::FullMove, 4> m_last_moves{};
   Tree m_tree;
   mb::size m_player_count;
   SimulationType m_simulation_type{};
 public:
   explicit MCTSPlayer(game::Game &game, Player player, SimulationType sim_type);

   void prepare_tree(const game::Game &game);
   void make_move(game::Game &game) noexcept;
};

} // namespace carcassonne::ai

#endif //CARCASSONNE_GPU_MCTS_PLAYER_H
