#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/AI/DeepRLPlayer.h>
#include <Carcassonne_GPU/AI/Tree.h>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace carcassonne::ai {

DEVHOST DeepRLPlayer::DeepRLPlayer(game::Game &game, Player player, carcassonne::rl::Network &net) : m_player(player),
                                                                                        m_tree(game, player),
                                                                                        m_player_count(game.player_count()),
                                                                                        m_network(net) {
   spdlog::info("deep rl: initialising agent");
   game.on_next_move([this](game::Game &game, Player player, game::FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

DEVHOST void DeepRLPlayer::prepare_tree(const game::Game &game) {
   auto player = m_player;
   NodeId node_id = 0;
   do {
      node_id = m_tree.find_node_by_move(node_id, m_last_moves[static_cast<mb::size>(player)]);
      if (node_id == 0)
         break;
      player = next_player(player, m_player_count);
   } while (player != m_player);

   if (node_id == 0) {
      spdlog::debug("deep rl: building MCTS tree from scratch");
      m_tree.reset(game, m_player);
      return;
   }

   spdlog::debug("deep rl: reusing existing MCTS tree node");
   m_tree.change_root(node_id);
}

DEVHOST void DeepRLPlayer::make_move(game::Game &game) noexcept {
   spdlog::info("deep rl: preparing move");

   prepare_tree(game);
   Context ctx{
           .tree = m_tree,
           .network = m_network,
   };
   run_mcts(ctx, 2000);
   auto tile = game.tile_set()[game.move_index()];
   auto best_move = choose_move(ctx, game.move_index(), m_player);
   m_last_moves[static_cast<int>(m_player)] = best_move;

   while (!game.can_place_tile_and_figure(best_move.x, best_move.y, best_move.rotation, tile, best_move.direction)) {
      spdlog::info("deep rl: selected incorrect move, running MCTS again");
      run_mcts(ctx, 1000);
      best_move = choose_move(ctx, game.move_index(), m_player);
      m_last_moves[static_cast<int>(m_player)] = best_move;
   }

   auto move = game.new_move(m_player);
   if (move.place_tile_at(best_move.x, best_move.y, best_move.rotation)) {
      // spdlog::error("deep rl: selected tile placement is not feasible: {}", res.msg());
      return;
   }

   if (move.phase() == game::MovePhase::Done)
      return;

   if (best_move.ignored_figure) {
      if (move.ignore_figure()) {
         // spdlog::error("deep rl: cannot ignore figure at this location: {}", res.msg());
      }
      return;
   }

   if (move.place_figure(best_move.direction)) {
      // spdlog::error("deep rl: error placing figure: {}", res.msg());
      return;
   }
}

}// namespace carcassonne::ai