#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/Tree.h>
#include <fmt/core.h>

namespace carcassonne::ai {

MCTSPlayer::MCTSPlayer(IGame &game, Player player) : m_player(player) {
   game.on_next_move([this](IGame &game, Player player) {
      if (player != m_player)
         return;
      make_move(game);
   });
}

void MCTSPlayer::make_move(IGame &game) noexcept {
   Tree tree(game, m_player);
   auto move = game.new_move(m_player);
   mb::u64 rollout_count = 0;
   auto best_move = tree.find_best_move(game, rollout_count);

   if (auto res = move->place_tile_at(best_move.x, best_move.y, best_move.rotation); !res.ok()) {
      fmt::print("[MCTS internal error] selected tile placement is not feasible: {}\n", res.msg());
      return;
   }

   if (best_move.ignored_figure) {
      if (auto res = move->ignore_figure(); !res.ok()) {
         fmt::print("[MCTS internal error] cannot ignore figure at this point: {}\n", res.msg());
      }
      return;
   }

   if (auto res = move->place_figure(best_move.direction); !res.ok()) {
      fmt::print("[MCTS internal error] error placing figure: {}\n", res.msg());
   }
}

}// namespace carcassonne::ai