#ifndef MSI_CARCASSONNE_TREE_H
#define MSI_CARCASSONNE_TREE_H
#include "Node.h"
#include <Carcassonne/Player.h>
#include <Carcassonne/IGame.h>
#include <mb/int.h>
#include <tuple>
#include <boost/pool/pool.hpp>

namespace carcassonne::ai {

class Tree {
 public:
   std::unique_ptr<Node> m_root;
   std::mutex m_tree_mutex;
   std::unique_lock<std::mutex> lck;
   
   Tree(IGame &game, const Player &player);
   Tree(Tree&&) noexcept;
   void change_root(NodePtr new_root_id);
   NodePtr find_node_by_move(NodePtr base_id, const FullMove &move);
   NodePtr add_node(rl::PoolGame &&game, Player player, FullMove move, float P, NodePtr parent_id);
   void reset(IGame &game, Player player);

   [[nodiscard]] inline mb::size node_count() {
      lck.lock();
      auto count = m_root->count_children();
      lck.unlock();
      return count;
   }

   [[nodiscard]] inline NodePtr root() const {
      return m_root.get();
   }
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_TREE_H