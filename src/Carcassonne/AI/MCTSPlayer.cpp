#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/MCTS.h>
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
//   auto best_move_res = find_non_idiotic(tree, m_player);
//   if (!best_move_res.ok()) {
//      fmt::print("[MCTS internal error] could not find move: {}\n", best_move_res.msg());
//      return;
//   }
//   auto best_move = best_move_res.unwrap();
   Tree tree(game, m_player);
   run_mcts(tree, 5000);
   auto best_move = choose_move(tree);

   auto move = game.new_move(m_player);
   if (auto res = move->place_tile_at(best_move.x, best_move.y, best_move.rotation); !res.ok()) {
      fmt::print("[MCTS internal error] selected tile placement is not feasible: {}\n", res.msg());
      return;
   }

   if (move->phase() == MovePhase::Done)
      return;

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