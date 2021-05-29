#include <Carcassonne/AI/Tree.h>
#include <chrono>
#include <cmath>
#include <limits>

namespace carcassonne::ai {

Tree::Tree(IGame &game, const Player &player)
 : m_player(player)
 , m_root(Node(game, player, m_simulations_count))
{
   find_best_move(game);
}

void Tree::find_best_move(const IGame &game) {

   using namespace std::literals;
   const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   const std::chrono::time_point<std::chrono::system_clock> end;
   for (
      auto start = std::chrono::steady_clock::now(), now = start; 
      now < start + std::chrono::milliseconds{1};
      now = std::chrono::steady_clock::now() )
   {
      selection();
      backpropagation();
      m_simulations_count++;
   }
}

std::tuple<TileMove, Direction> Tree::best_move(IGame &game) noexcept {
   TileMove tile_placement;
   Direction figure_placement;
   // BEST_MOVE
   return std::make_tuple(tile_placement, figure_placement);
}

mb::u64 Tree::selection() {
   // TODO: SELECTION
   
   return 0;
}

void Tree::backpropagation() {
   // BACKPROPAGATION
}



}// namespace carcassonne::ai