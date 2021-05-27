#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <mb/int.h>
#include "Node.h"
#include <tuple>

namespace carcassonne::ai {

class Tree {
   carcassonne::game::Game m_game;
   Player m_player;
   mb::u64 m_selected_node_id;
   mb::u64 m_simulations_count;
   double g_C; // TODO: move C to global parameters location
 public:
   Tree(const game::Game &game, const Player &player);
   void find_best_move();
   [[nodiscard]] std::tuple<TileMove, Direction> best_move(const game::Game &game) noexcept;
   mb::u64 selection();
   std::vector<Node> expansion();
   short simulation(game::Game &game);
   void backpropagation();
   double UCT1(Node node);
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_TREE_H