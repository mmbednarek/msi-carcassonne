#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/MCTS.h>
#include <Carcassonne/AI/Tree.h>
#include <fmt/core.h>

namespace carcassonne::ai {

MCTSPlayer::MCTSPlayer(IGame &game, Player player) : m_player(player), m_tree(game, player), m_player_count(game.player_count()) {
   game.on_next_move([this](IGame &game, Player player, FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

void MCTSPlayer::prepare_tree(const IGame &game) {
   auto player = m_player;
   NodeId node_id = 0;
   do {
      node_id = m_tree.find_node_by_move(node_id, m_last_moves[static_cast<mb::size>(player)]);
      if (node_id == 0)
         break;
      player = next_player(player, m_player_count);
   } while(player != m_player);

   if (node_id == 0) {
      fmt::print("node not in MCTS building a new tree\n");
      m_tree.reset(game, m_player);
      return;
   }

   fmt::print("node found in existing MCTS tree!\n");
   m_tree.change_root(node_id);
}

void MCTSPlayer::make_move(IGame &game) noexcept {
   prepare_tree(game);
   run_mcts(m_tree, 3000);
   auto best_move = choose_move(m_tree, game.move_index(), m_player);
   m_last_moves[static_cast<int>(m_player)] = best_move;

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