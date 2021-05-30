#include <Carcassonne/AI/Tree.h>
#include <chrono>
#include <cmath>
#include <limits>

namespace carcassonne::ai {

Tree::Tree(IGame &game, const Player &player)
 : m_player(player)
 , m_root(Node(game, player, m_rollouts_performed_count))
{
   m_root.expansion(m_rollouts_performed_count);
   find_best_move(game);
}

void Tree::find_best_move(const IGame &game) {
   selection(m_root);
   using namespace std::literals;
   const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   const std::chrono::time_point<std::chrono::system_clock> end;
   for (
      auto start = std::chrono::steady_clock::now(), now = start; 
      now < start + std::chrono::milliseconds{1};
      now = std::chrono::steady_clock::now() )
   {
      // selection();
      backpropagation();
      m_rollouts_performed_count++;
   }
}

std::tuple<TileMove, Direction> Tree::best_move(IGame &game) noexcept {
   TileMove tile_placement;
   Direction figure_placement;
   // BEST_MOVE
   return std::make_tuple(tile_placement, figure_placement);
}

mb::u64 Tree::selection(Node &current_node) {
   // TODO: SELECTION
   double best_uct = 0;
   auto selected_node = std::max_element(
      current_node.children().begin(), 
      current_node.children().end(), 
      [](const std::unique_ptr<Node> &n_uptr1, const std::unique_ptr<Node> &n_uptr2) -> bool {
      return (*(n_uptr1)).UCT1() < (*(n_uptr2)).UCT1();
   });
   if (selected_node == current_node.children().end())
      return 0;
   if((*(*selected_node)).children().size() == 0) { // is it a leaf node?
      if((*(*selected_node)).m_visitation_count == 0) {
         std::tie(current_node.m_wins_count, current_node.m_loses_count) = (*(*selected_node)).simulation();
      }
   }
   return 0;
}

void Tree::backpropagation() {
   // BACKPROPAGATION
}



}// namespace carcassonne::ai