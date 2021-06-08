#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Util/Random.h>
#include <fmt/core.h>

namespace carcassonne::ai {

void HeuristicPlayer::await_turn(IGame &game) {
   game.on_next_move([this](IGame &game, Player player, FullMove) {
      if (player != m_player)
         return;
      make_move(game);
   });
}

mb::result<FullMove> HeuristicPlayer::make_move(IGame &game) noexcept {
   auto move = game.new_move(m_player);

   Direction best_dir{};
   auto best_score = std::numeric_limits<int>::min();
   TileMove best_move{};
   const auto possible_tile_moves = game.moves(move->tile_type());
   for (const auto possible_move : possible_tile_moves) {
      int score;
      Direction dir;
      std::tie(dir, score) = game.move_score(m_player, move->tile_type(), possible_move);
      if (score > best_score) {
         best_dir = dir;
         best_score = score;
         best_move = possible_move;
      }
   }

   MB_TRY(move->place_tile(best_move));
   if (move->phase() == MovePhase::Done) {
      return FullMove{
              .x = best_move.x,
              .y = best_move.y,
              .rotation = best_move.rotation,
              .ignored_figure = true,
      };
   }

   if (best_score < game.params().ignore_figure_score_treshold) {
      move->ignore_figure();
      return FullMove{
              .x = best_move.x,
              .y = best_move.y,
              .rotation = best_move.rotation,
              .ignored_figure = true,
      };
   }

   if (auto res = move->place_figure(best_dir); !res.ok()) {
      move->ignore_figure();
      return FullMove{
              .x = best_move.x,
              .y = best_move.y,
              .rotation = best_move.rotation,
              .ignored_figure = true,
      };
   }

   return FullMove{
           .x = best_move.x,
           .y = best_move.y,
           .rotation = best_move.rotation,
           .ignored_figure = false,
           .direction = best_dir,
   };
}

}// namespace carcassonne::ai