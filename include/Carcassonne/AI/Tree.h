#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include <Carcassonne/IGame.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <mb/int.h>
#include "Node.h"
#include <tuple>

namespace carcassonne::ai {

class Tree {
   Player m_player;
   mb::u64 m_selected_node_id;
   mb::u64 m_simulations_count;
   double g_C; // TODO: move C to global parameters location
 public:
   Tree(std::unique_ptr<IGame> game, const Player &player);
   void find_best_move(IGame &game);
   [[nodiscard]] std::tuple<TileMove, Direction> best_move(IGame &game) noexcept;
   mb::u64 selection();
   std::vector<Node> expansion();
   short simulation(IGame &game);
   void backpropagation();
   double UCT1(Node node);
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_TREE_H