#include <Carcassonne/AI/Tree.h>
#include <chrono>
#include <cmath>
#include <limits>

namespace carcassonne::ai {

Tree::Tree(IGame &game, const Player &player) : m_player(player), m_root(Node(game, player)) {
   find_best_move(game);
}

void Tree::find_best_move(const IGame &game) {
   auto g = game.clone();

   using namespace std::literals;
   const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   const std::chrono::time_point<std::chrono::system_clock> end;
   for (
      auto start = std::chrono::steady_clock::now(), now = start; 
      now < start + std::chrono::milliseconds{1};
      now = std::chrono::steady_clock::now() )
   {
      selection();
      // expansion();
      simulation(*g);
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
   return m_selected_node_id;
}

short Tree::simulation(IGame &game) {
   // IN PROGRESS
   RandomPlayer rp = RandomPlayer(game, m_player);
   auto it = std::find_if(game.scores().begin(), game.scores().end(),
                          [this](PlayerScore score) { return score.player == this->m_player; });
   if (it == game.scores().end()) {
      return 0;
   }
   return it->score;
}

void Tree::backpropagation() {
   // BACKPROPAGATION
}

double Tree::UCT1(Node node) {
   // DONE
   if (node.m_visitation_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return node.m_profits_sum / this->m_simulations_count + g_C * sqrt(log(this->m_simulations_count) / node.m_visitation_count);
}

}// namespace carcassonne::ai