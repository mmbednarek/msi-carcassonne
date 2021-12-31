#include <Carcassonne/AI/DeepRLPlayer.h>
#include <Carcassonne/AI/Tree.h>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace carcassonne::ai {

DeepRLPlayer::DeepRLPlayer(IGame &game, Player player, carcassonne::rl::Network &net) : m_player(player),
                                                                                        m_tree(game, player),
                                                                                        m_player_count(game.player_count()),
                                                                                        m_network(net) {
   spdlog::info("deep rl: initialising agent");
   game.on_next_move([this](IGame &game, Player player, FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

void DeepRLPlayer::prepare_tree(const IGame &game) {
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

void DeepRLPlayer::make_move(IGame &game) noexcept {
   spdlog::info("deep rl: preparing move");

   prepare_tree(game);
   rl::Context ctx{
           .tree = m_tree,
           .network = m_network,
   };
   auto tile = game.tile_set()[game.move_index()];
   FullMove best_move;
   bool move_is_illegal = true;
   do {
      rl::run_mcts(ctx, 1000);
      best_move = rl::choose_move(ctx, game.move_index(), m_player);
      m_last_moves[static_cast<int>(m_player)] = best_move;
      if (best_move.ignored_figure) {
         if (game.board().can_place_at(best_move.x, best_move.y, game.tile_set()[game.move_index()], best_move.rotation)) {
            move_is_illegal = false;
            continue;
         }
      } else if (game.can_place_tile_and_figure(best_move.x, best_move.y, best_move.rotation, game.tile_set()[game.move_index()], best_move.direction)) {
         move_is_illegal = false;
         continue;
      }
      spdlog::info("deep rl: selected incorrect move, running MCTS again");
   } while (move_is_illegal);

   auto move = game.new_move(m_player);
   if (auto res = move->place_tile_at(best_move.x, best_move.y, best_move.rotation); !res.ok()) {
      spdlog::error("deep rl: selected tile placement is not feasible: {}", res.msg());
      return;
   }

   if (move->phase() == MovePhase::Done)
      return;

   if (best_move.ignored_figure) {
      if (auto res = move->ignore_figure(); !res.ok()) {
         spdlog::error("deep rl: cannot ignore figure at this location: {}", res.msg());
      }
      return;
   }

   if (auto res = move->place_figure(best_move.direction); !res.ok()) {
      spdlog::error("deep rl: error placing figure: {}", res.msg());
   }
}

}// namespace carcassonne::ai