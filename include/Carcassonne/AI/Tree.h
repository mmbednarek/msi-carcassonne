#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include "Node.h"
#include <Carcassonne/Player.h>
#include <Carcassonne/IGame.h>
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {

constexpr NodeId g_root_node = 0;

class Tree {
   std::vector<Node> m_nodes;
 public:
   Tree(const IGame &game, const Player &player);
   void change_root(NodeId new_root_id);
   NodeId find_node_by_move(NodeId base_id, const FullMove &move);
   mb::size add_node(std::unique_ptr<IGame> &&game, Player player, FullMove move, NodeId parent_id);
   Node &node_at(NodeId id) noexcept;
   void reset(const IGame &game, Player player);

   [[nodiscard]] inline mb::size node_count() const noexcept {
      return m_nodes.size();
   }
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_TREE_H