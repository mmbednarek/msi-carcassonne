#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include <Carcassonne/IGame.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include "Node.h"
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {


class Tree {
  std::unique_ptr<Node> m_root;
  Player m_player;
  mb::u64 m_rollouts_performed_count = 0;
 public:
  Tree(IGame &game, const Player &player);
  void find_best_move(const IGame &game, mb::u64 &rollouts_performed_count);
  [[nodiscard]] std::tuple<TileMove, Direction> best_move(IGame &game) noexcept;
  [[nodiscard]] std::unique_ptr<Node> selection(const std::unique_ptr<Node> &current_node, mb::u64 &rollouts_performed_count);
  void backpropagation(const std::unique_ptr<Node> &node);
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_TREE_H