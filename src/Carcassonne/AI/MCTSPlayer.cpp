#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/Tree.h>
#include <Util/Random.h>
#include <fmt/core.h>

namespace carcassonne::ai {

MCTSPlayer::MCTSPlayer(IGame &game, Player player) : m_player(player), m_random_generator(m_random_device()) {
   game.on_next_move([this](IGame &game, Player player) {
     if (player != m_player)
         return;
      make_move(game);
   });
}

void MCTSPlayer::make_move(IGame &game) noexcept {
   auto move = game.new_move(m_player);

   const auto possible_tile_moves = game.moves(move->tile_type());
   // TileMove tile_placement;
   Direction figure_placement;
   // Tree tree(game);
   // std::tie(tile_placement, figure_placement) = tree.best_move();
   auto tile_placement_it = util::random_from_range(m_random_generator, possible_tile_moves.begin(), possible_tile_moves.end() - 1);
   const auto tile_placement = *tile_placement_it;

   move->place_tile(tile_placement.x, tile_placement.y, tile_placement.rotation);
   if (move->phase() == MovePhase::Done) {
      return;
   }
   // move->place_figure(figure_placement);

   const auto possible_figure_moves = game.figure_placements(tile_placement.x, tile_placement.y);
   if (!possible_figure_moves.empty()) {
      const auto direction_it = util::random_from_range(m_random_generator, possible_figure_moves.cbegin(), possible_figure_moves.cend());
      if (direction_it == possible_figure_moves.cend()) {
         move->ignore_figure();
      } else {
         move->place_figure(*direction_it);
      }
   } else {
      move->ignore_figure();
   }
}

}// namespace carcassonne::ai