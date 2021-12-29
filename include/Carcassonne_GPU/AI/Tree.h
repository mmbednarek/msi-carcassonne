#ifndef CARCASSONNE_GPU_TREE_H
#define CARCASSONNE_GPU_TREE_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include "Node.h"
#include <Carcassonne_GPU/Player.h>
#include <Carcassonne_GPU/Game/Game.h>
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {

DEVHOST constexpr NodeId g_root_node = 0;

class Tree {
   std::vector<Node> m_nodes;
 public:
   Tree(const game::Game &game, const Player &player);
   void change_root(NodeId new_root_id);
   NodeId find_node_by_move(NodeId base_id, const game::FullMove &move);
   mb::size add_node(std::unique_ptr<game::Game> &&game, Player player, game::FullMove move, NodeId parent_id);
   Node &node_at(NodeId id) noexcept;
   void reset(const game::Game &game, Player player);

   [[nodiscard]] DEVHOST inline mb::size node_count() const noexcept {
      return m_nodes.size();
   }
};

}// namespace carcassonne::ai

#endif//CARCASSONNE_GPU_TREE_H