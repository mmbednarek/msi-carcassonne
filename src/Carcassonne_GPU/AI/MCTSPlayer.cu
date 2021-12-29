#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/AI/MCTSPlayer.h>
#include <Carcassonne_GPU/AI/Tree.h>
#include <fmt/core.h>

namespace carcassonne::ai {

DEVHOST MCTSPlayer::MCTSPlayer(game::Game &game, Player player, SimulationType sim_type) : m_player(player), m_tree(game, player), m_player_count(game.player_count()), m_simulation_type(sim_type) {
   game.on_next_move([this](game::Game &game, Player player, game::FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

DEVHOST void MCTSPlayer::prepare_tree(const game::Game &game) {
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

DEVHOST void MCTSPlayer::make_move(game::Game &game) noexcept {
   prepare_tree(game);
   run_mcts(m_tree, 2000, m_simulation_type);
   auto best_move = choose_move(m_tree, game.move_index(), m_player);
   m_last_moves[static_cast<int>(m_player)] = best_move;

   auto move = game.new_move(m_player);
   if (move.place_tile_at(best_move.x, best_move.y, best_move.rotation)) {
      // fmt::print("[MCTS internal error] selected tile placement is not feasible: {}\n", res.msg());
      return;
   }

   if (move.phase() == game::MovePhase::Done)
      return;

   if (best_move.ignored_figure) {
      if (move.ignore_figure()) {
         // fmt::print("[MCTS internal error] cannot ignore figure at this point: {}\n", res.msg());
      }
      return;
   }

   if (move.place_figure(best_move.direction)) {
      // fmt::print("[MCTS internal error] error placing figure: {}\n", res.msg());
      return;
   }
}

}// namespace carcassonne::ai