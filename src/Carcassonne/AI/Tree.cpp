#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/RL/GamePool.h>
#include <cassert>
#include <cmath>
#include <fmt/core.h>
#include <queue>

namespace carcassonne::ai {

Tree::Tree(IGame &game, const Player &player) : m_root(std::make_unique<Node>(rl::PoolGame(game), player, FullMove{})) {
   lck = std::unique_lock<std::mutex>{m_tree_mutex, std::defer_lock};
}

Tree::Tree(Tree &&t) noexcept : m_root(std::move(t.m_root)),
                                lck(std::unique_lock<std::mutex>{m_tree_mutex, std::defer_lock}) {
}

struct NodePair {
   NodePtr parent_id;
   NodePtr old_id;

   constexpr bool operator<(const NodePair &other) const {
      return old_id > other.old_id;
   }
};

void Tree::change_root(NodePtr new_root_id) {
   auto &children = new_root_id->parent()->children();
   auto it = std::find_if(children.begin(), children.end(), [new_root_id](const std::unique_ptr<Node> &node) {
      return new_root_id == node.get();
   });
   assert(it != children.end());
   auto tmp_root = std::move(*it);
   m_root = std::move(tmp_root);
   m_root->mark_as_root();
}

NodePtr Tree::add_node(ai::rl::PoolGame &&game, Player player, FullMove move, float P, NodePtr parent_id) {
   return parent_id->add_child(std::move(game), player, move, P);
}

NodePtr Tree::find_node_by_move(NodePtr base_id, const FullMove &move) {
   auto &children = base_id->children();
   auto node_it = std::find_if(children.begin(), children.end(), [move](const std::unique_ptr<Node> &node_id) {
      return node_id->move() == move;
   });

   if (node_it == children.end())
      return nullptr;

   return node_it->get();
}

void Tree::reset(IGame &game, Player player) {
   m_root = std::make_unique<Node>(rl::PoolGame(game), player, FullMove{});
}


}// namespace carcassonne::ai