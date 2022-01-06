#include <Carcassonne/AI/Tree.h>
#include <cmath>
#include <fmt/core.h>
#include <queue>
#include <cassert>

namespace carcassonne::ai {

Tree::Tree(const IGame &game, const Player &player) {
   m_nodes.emplace_back(game.clone(), player, FullMove{});
   lck = std::unique_lock<std::mutex>{m_tree_mutex, std::defer_lock};
}

Tree::Tree(Tree&& t) : m_nodes(std::move(t.m_nodes)), m_tree_mutex() {
   lck = std::unique_lock<std::mutex>{m_tree_mutex, std::defer_lock};
}

struct NodePair {
   NodeId parent_id;
   NodeId old_id;

   constexpr bool operator<(const NodePair &other) const {
      return old_id > other.old_id;
   }
};

void Tree::change_root(NodeId new_root_id) {
   std::priority_queue<NodePair> queue;
   queue.push(NodePair{0, new_root_id});

   mb::size node_count = 0;
   while (!queue.empty()) {
      auto pair = queue.top();
      queue.pop();

      assert(pair.old_id >= node_count);

      auto &node = m_nodes[pair.old_id];
      const auto &children = node.children();
      for (const auto child_id : children) {
         queue.push(NodePair{node_count, child_id});
      }
      node.clear_children();

      node.update_id(node_count);
      node.update_parent_id(pair.parent_id);

      if (pair.old_id != 0) {
         auto &parent_node = m_nodes[pair.parent_id];
         parent_node.add_child(node_count);
      }
      m_nodes[node_count] = std::move(node);
      ++node_count;
   }

   m_nodes.erase(m_nodes.begin() + static_cast<long>(node_count), m_nodes.end());
}

mb::size Tree::add_node(std::unique_ptr<IGame> &&game, Player player, FullMove move, float P, NodeId parent_id) {
   auto node_id = m_nodes.size();
   m_nodes.emplace_back(node_id, std::move(game), player, move, P, parent_id);
   m_nodes[parent_id].add_child(node_id);
   return node_id;
}

Node &Tree::node_at(NodeId id) noexcept {
   return m_nodes[id];
}

NodeId Tree::find_node_by_move(NodeId base_id, const FullMove &move) {
   auto &children = m_nodes[base_id].children();
   auto node_it = std::find_if(children.begin(), children.end(), [move, this](NodeId &node_id) {
      return m_nodes[node_id].move() == move;
   });
   if (node_it == children.end()) {
      return 0;
   }
   return *node_it;
}

void Tree::reset(const IGame &game, Player player) {
   m_nodes.clear();
   m_nodes.emplace_back(game.clone(), player, FullMove{});
}


}// namespace carcassonne::ai