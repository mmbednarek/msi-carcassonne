#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include <Carcassonne/IGame.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include "Node.h"
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {

using RefNode = std::reference_wrapper<Node>;

class Tree {
  Node m_root;
  Player m_player;
  mb::u64 m_rollouts_performed_count = 0;
  std::vector<Node> m_nodes;
 public:
  Tree(IGame &game, const Player &player);
  void find_best_move(const IGame &game, mb::u64 &rollouts_performed_count);
  [[nodiscard]] std::tuple<TileMove, Direction> best_move(IGame &game) noexcept;
  [[nodiscard]] RefNode selection(RefNode current_node, mb::u64 &rollouts_performed_count);
  void expansion(RefNode selected_node, mb::u64 &rollouts_performed_count) noexcept;
  void backpropagation(RefNode node);
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_TREE_H