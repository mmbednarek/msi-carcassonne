#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include <Carcassonne/IGame.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include "Node.h"
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {


class Tree {
  Node m_root;
  Player m_player;
  mb::u64 m_simulations_count = 0;
 public:
  Tree(IGame &game, const Player &player);
  void find_best_move(const IGame &game);
  [[nodiscard]] std::tuple<TileMove, Direction> best_move(IGame &game) noexcept;
  mb::u64 selection();
  void backpropagation();
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_TREE_H