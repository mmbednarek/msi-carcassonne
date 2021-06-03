#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Util/Random.h>
#include <fmt/core.h>

namespace carcassonne::ai {

void HeuristicPlayer::await_turn(IGame &game) {
   game.on_next_move([this](IGame &game, Player player) {
     if (player != m_player)
        return;
     make_move(game);
   });
}

void HeuristicPlayer::make_move(IGame &game) noexcept {
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

   if (auto res = move->place_tile(best_move); !res.ok()) {
      fmt::print("cannot place tile: {}\n", res.msg());
      return;
   }
   if (move->phase() == MovePhase::Done) {
      return;
   }

   if (best_score < 0) {
      move->ignore_figure();
      return;
   }

   if (auto res = move->place_figure(best_dir); !res.ok()) {
      move->ignore_figure();
//      fmt::print("cannot place figure: {}\n", res.msg());
   }
}

}