#include <Carcassonne/AI/Tree.h>
#include <chrono>
#include <cmath>
#include <limits>

namespace carcassonne::ai {

Tree::Tree(IGame &game, const Player &player)
 : m_player(player)
 , m_root(std::make_unique<Node>(game, player, m_rollouts_performed_count))
{
   (*m_root).expansion(m_rollouts_performed_count);
   find_best_move(game, m_rollouts_performed_count);
}

void Tree::find_best_move(const IGame &game, mb::u64 &rollouts_performed_count) {
   std::unique_ptr<Node> node_ptr = selection(m_root, rollouts_performed_count);
   using namespace std::literals;
   const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   const std::chrono::time_point<std::chrono::system_clock> end;
   for (
      auto start = std::chrono::steady_clock::now(), now = start; 
      now < start + std::chrono::milliseconds{1};
      now = std::chrono::steady_clock::now() )
   {
      // backpropagation(node_ptr);
      node_ptr = selection(m_root, rollouts_performed_count);
      m_rollouts_performed_count++;
   }
}

std::tuple<TileMove, Direction> Tree::best_move(IGame &game) noexcept {
   TileMove tile_placement;
   Direction figure_placement;
   // BEST_MOVE
   return std::make_tuple(tile_placement, figure_placement);
}

std::unique_ptr<Node> Tree::selection(const std::unique_ptr<Node> &current_node, mb::u64 &rollouts_performed_count) {
   double best_uct = 0;
   std::unique_ptr<Node> node_ptr;
   auto selected_node = std::max_element(
      (*current_node).children().begin(), 
      (*current_node).children().end(), 
      [](const std::unique_ptr<Node> &n_uptr1, const std::unique_ptr<Node> &n_uptr2) -> bool {
      return (*(n_uptr1)).UCT1() < (*(n_uptr2)).UCT1();
   });
   if (selected_node == (*current_node).children().end())
      return nullptr;
   if((*(*selected_node)).children().size() == 0) { // is it a leaf node?
      if((*(*selected_node)).m_visitation_count == 0) {
         std::tie((*current_node).m_wins_count, (*current_node).m_loses_count) = (*(*selected_node)).simulation();
         rollouts_performed_count++;
      } else {
         (*(*selected_node)).expansion(rollouts_performed_count);
         node_ptr = selection(*selected_node, rollouts_performed_count);
      }
   } else {
      node_ptr = selection(*selected_node, rollouts_performed_count);
   }
   return node_ptr;
}

void Tree::backpropagation(const std::unique_ptr<Node> &node) {
   // BACKPROPAGATION
}



}// namespace carcassonne::ai