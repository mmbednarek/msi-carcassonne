#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/AI/Tree.h>
#include <cmath>
#include <fmt/core.h>
#include <queue>
#include <cassert>

namespace carcassonne::ai {

DEVHOST Tree::Tree(const game::Game &game, const Player &player) {
   m_nodes.emplace_back(game::Game{game}, player, game::FullMove{});
}

DEVHOST struct NodePair {
   NodeId parent_id;
   NodeId old_id;

   constexpr bool operator<(const NodePair &other) const {
      return old_id > other.old_id;
   }
};

DEVHOST void Tree::change_root(NodeId new_root_id) {
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

DEVHOST mb::size Tree::add_node(std::unique_ptr<game::Game> &&game, Player player, game::FullMove move, NodeId parent_id) {
   auto node_id = m_nodes.size();
   m_nodes.emplace_back(node_id, std::move(game), player, move, parent_id);
   m_nodes[parent_id].add_child(node_id);
   return node_id;
}

DEVHOST Node &Tree::node_at(NodeId id) noexcept {
   return m_nodes[id];
}

DEVHOST NodeId Tree::find_node_by_move(NodeId base_id, const game::FullMove &move) {
   auto &children = m_nodes[base_id].children();
   auto node_it = std::find_if(children.begin(), children.end(), [move, this](NodeId &node_id) {
      return m_nodes[node_id].move() == move;
   });
   if (node_it == children.end()) {
      return 0;
   }
   return *node_it;
}

DEVHOST void Tree::reset(const game::Game &game, Player player) {
   m_nodes.clear();
   m_nodes.emplace_back(game::Game{game}, player, game::FullMove{});
}


}// namespace carcassonne::ai