#include <Carcassonne/AI/Tree.h>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fmt/core.h>

namespace carcassonne::ai {

Tree::Tree(const IGame &game, const Player &player) {
   m_nodes.emplace_back(game.clone(), player, FullMove{});
}

void Tree::change_root(NodeId id) {
   std::for_each(m_nodes.begin(), m_nodes.end(), [](Node &node) {
      node.clear_children();
   });

   std::vector<bool> to_erase(m_nodes.size());
   std::transform(m_nodes.begin(), m_nodes.end(), to_erase.begin(), [this, new_id = 0, expected = id](const Node &node) mutable {
      auto child = node.id();
      while (child != 0) {
         if (child == expected) {
            auto &child_node = m_nodes[expected];
            auto &parent_node = m_nodes[child_node.parent_id()];
            child_node.update_id(new_id);
            child_node.update_parent_id(parent_node.id());
            parent_node.add_child(new_id);
            ++new_id;
            return false;
         }
         child = m_nodes[expected].id();
      }
      return true;
   });

   std::erase_if(m_nodes, [&to_erase, index = 0](const Node &node) mutable {
      return to_erase[index++];
   });
}

mb::size Tree::add_node(std::unique_ptr<IGame> &&game, Player player, FullMove move, NodeId parent_id) {
   auto node_id = m_nodes.size();
   m_nodes.emplace_back(node_id, std::move(game), player, move, parent_id);
   m_nodes[parent_id].add_child(node_id);
   return node_id;
}

Node &Tree::node_at(NodeId id) noexcept {
   return m_nodes[id];
}


}// namespace carcassonne::ai