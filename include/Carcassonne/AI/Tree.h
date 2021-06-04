#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include "Node.h"
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/IGame.h>
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {

class Tree {
   std::vector<Node> m_nodes;
   Player m_player;
   mb::size m_player_count;

 public:
   Tree(const IGame &game, const Player &player);
   FullMove find_non_idiotic();
   FullMove find_best_move();
   void selection(NodeId node_id, int level);
   void expansion(NodeId node_id) noexcept;
   void backpropagation(NodeId node_id);
   void change_root(NodeId id);

 private:
   [[nodiscard]] NodeId select_best_node();
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_TREE_H