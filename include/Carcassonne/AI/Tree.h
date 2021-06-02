#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include "Node.h"
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/IGame.h>
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {

using RefNode = std::reference_wrapper<Node>;

class Tree {
   std::vector<Node> m_nodes;
   Player m_player;
   mb::u64 m_rollouts_performed_count = 0;

 public:
   Tree(const IGame &game, const Player &player);
   FullMove find_best_move(const IGame &game, mb::u64 &rollouts_performed_count);
   [[nodiscard]] NodeId selection(NodeId, mb::u64 &rollouts_performed_count);
   void expansion(NodeId node_id, mb::u64 &rollouts_performed_count) noexcept;
   void backpropagation(Node &node);
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_TREE_H